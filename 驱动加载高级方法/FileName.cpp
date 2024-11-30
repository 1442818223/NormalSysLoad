#pragma once
#include<windows.h>
#include<stdio.h>
#include<winternl.h>
#include<psapi.h>
#include <time.h>
#include <iostream>
#include <tchar.h>
#include <TlHelp32.h>
#pragma comment(lib,"ntdll.lib")
using namespace std;
typedef VOID(__stdcall* RtlInitUnicodeStringPtr) (IN OUT PUNICODE_STRING  DestinationString, IN wchar_t* SourceString);
typedef NTSTATUS(WINAPI* RtlAdjustPrivilegePtr)(IN ULONG Privilege, IN BOOL Enable, IN BOOL CurrentThread, OUT PULONG pPreviousState);
typedef NTSTATUS(WINAPI* NtLoadDriverPtr)(const UNICODE_STRING*);
typedef NTSTATUS(WINAPI* NtUnLoadDriverPtr)(const UNICODE_STRING*);
typedef void(__stdcall* NTPROC)(DWORD*, DWORD*, DWORD*);
TCHAR* RandomName = NULL;
CHAR* DrivePath = NULL;
HANDLE hDevice = NULL;
BOOL LoadDriver() {
	HKEY hKey;
	HKEY hsubkey;
	// �� "System\\CurrentControlSet" ע����������������Ƶ����
	if (!RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet", 0, 2u, &hKey) && !RegCreateKeyW(hKey, RandomName, &hsubkey)) {
		// Ϊ ImagePath ׼�����ݲ�����ע���ֵ
		CHAR* pdata = (CHAR*)calloc(1024, 1);
		if (pdata == NULL) return FALSE;
		memcpy(pdata, "\\??\\", strlen("\\??\\")); // ��·��ǰ���� "\\??\\"��
		memcpy(pdata + strlen("\\??\\"), DrivePath, strlen(DrivePath));// ������·��ƴ�ӵ����档

		// ���� ImagePath ע���ֵ
		if (RegSetValueExA(hsubkey, "ImagePath", 0, REG_EXPAND_SZ, (PBYTE)pdata, (DWORD)(strlen(pdata) + 1))) {
			printf("Step1 Error\n");
			return FALSE;
		}
		// ������������Ϊ 1����ʾ�������ͣ���
		BYTE bDwod[4] = { 0 };
		*(DWORD*)bDwod = 1;
		if (RegSetValueExA(hsubkey, "Type", 0, 4u, bDwod, 4u)) {
			printf("Step2 Error\n");
			return FALSE;
		}

		// �� "System\\CurrentControlSet\\services" ע�����������
		if (!RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\services", 0, 2u, &hKey)) {
			RegCreateKeyW(hKey, RandomName, &hsubkey);
		}
		else {
			printf("Step3 Error\n");
			return FALSE;
		}
		RegCloseKey(hKey);

		// ��̬���� ntdll.dll����ȡ NtLoadDriver �Ⱥ���ָ�롣
		HMODULE hMoudle = LoadLibraryA("ntdll.dll");
		if (hMoudle == NULL) {
			printf("Step4 Error\n");
			return FALSE;
		}
		// ��ȡ����ָ��
		RtlInitUnicodeStringPtr RtlInitUnicodeString = (RtlInitUnicodeStringPtr)GetProcAddress(hMoudle, "RtlInitUnicodeString");
		NtLoadDriverPtr NtLoadDriver = (NtLoadDriverPtr)GetProcAddress(hMoudle, "NtLoadDriver");
		RtlAdjustPrivilegePtr RtlAdjustPrivilege = (RtlAdjustPrivilegePtr)GetProcAddress(hMoudle, "RtlAdjustPrivilege");
		ULONG previousState;
		NTSTATUS status = RtlAdjustPrivilege(0xa,// ����0xA: SE_LOAD_DRIVER_PRIVILEGE
			TRUE,//TRUE��������Ȩ��
			FALSE,//FALSE:3��
			&previousState);

		if (!NT_SUCCESS(status)) {
			printf("Step5 Error\n");
			return FALSE;
		}

		// ����������������
		UNICODE_STRING szSymbolicLink;
		TCHAR LinkPath[100] = L"\\Registry\\Machine\\System\\CurrentControlSet\\";
		lstrcat(LinkPath, RandomName);// ���������ƴ�ӵ�·�����档
		RtlInitUnicodeString(&szSymbolicLink, LinkPath);
		// ���Լ�������
		INT errcode = NtLoadDriver(&szSymbolicLink);//Ҳ����˵NtLoadDriver���Ը���ע��������ȥ��������
		if (errcode >= 0)
		{
			return TRUE;
		}
		else
		{ // �����������ʧ�ܵĴ�����
			if (errcode == 0xc0000603) {
				printf("The driver's certificate has been revoked, please wait for the project to be updated..\n");
			}
			else {
				printf("Error Code: % lx.\n", errcode);
			}

			return FALSE;
		}

	}
	else {
		printf("Reg Add Error, The program needs to be run with administrator privileges!\n");
		return FALSE;
	}
}
VOID UnloadDrive() {
	HMODULE hMoudle = LoadLibraryA("ntdll.dll");
	if (hMoudle == NULL) {
		printf("Unload Driver Error 1\n");
		return;
	}
	RtlAdjustPrivilegePtr RtlAdjustPrivilege = (RtlAdjustPrivilegePtr)GetProcAddress(hMoudle, "RtlAdjustPrivilege");
	ULONG previousState;
	NTSTATUS status = RtlAdjustPrivilege(0xa, TRUE, FALSE, &previousState);
	if (!NT_SUCCESS(status)) {
		printf("Unload Driver Error 2\n");
		return;
	}

	RtlInitUnicodeStringPtr RtlInitUnicodeString = (RtlInitUnicodeStringPtr)GetProcAddress(hMoudle, "RtlInitUnicodeString");
	UNICODE_STRING szSymbolicLink;
	TCHAR LinkPath[100] = L"\\Registry\\Machine\\System\\CurrentControlSet\\";
	lstrcat(LinkPath, RandomName);
	RtlInitUnicodeString(&szSymbolicLink, LinkPath);
	NtUnLoadDriverPtr NtUnLoadDriver = (NtUnLoadDriverPtr)GetProcAddress(hMoudle, "NtUnloadDriver");

	int errcode = NtUnLoadDriver(&szSymbolicLink);
	if (errcode >= 0)
	{
		printf("Driver uninstalled successfully.\n");
	}
	else {
		printf("Unload Driver Error: %lx\n", errcode);
	}
}

BOOL InitialDriver() {
	hDevice = CreateFile(L"\\\\.\\EchoDrv", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDevice == INVALID_HANDLE_VALUE) {
		if (LoadDriver()) {
			printf("Driver loaded successfully.\n");
			hDevice = CreateFile(L"\\\\.\\EchoDrv", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		}
		else {
			printf("Driver loading failed.\n");
			return FALSE;
		}
	}
}
typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
	HANDLE Section;
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
	ULONG NumberOfModules;
	RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

PVOID GetModuleBase(CHAR* Name) {
	PRTL_PROCESS_MODULES ModuleInfo = (PRTL_PROCESS_MODULES)calloc(1024 * 1024, 1);
	if (ModuleInfo == NULL) return 0;
	NTSTATUS status = NtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)11, ModuleInfo, 1024 * 1024, NULL);
	if (!NT_SUCCESS(status)) {
		return 0;
	}

	for (ULONG i = 0; i < ModuleInfo->NumberOfModules; i++)
	{
		if (lstrcmpiA((LPCSTR)(ModuleInfo->Modules[i].FullPathName + ModuleInfo->Modules[i].OffsetToFileName), Name) == 0) {

			return ModuleInfo->Modules[i].ImageBase;
		}
	}
	return 0;
}

// ��ȡָ��ģ����Ŀ�꺯�����ں�̬���Ե�ַ
INT64 GetFuncAddress(CHAR* ModuleName, CHAR* FuncName) {
	//��ȡģ�����ں˵�ַ�ռ��еĻ�ַ
	PVOID KBase = GetModuleBase(ModuleName);
	if (KBase == 0) {
		printf("ntoskrnl.exe base address not found.\n");
		return 0;
	}
	HMODULE ntos = NULL;
	if (strcmp(ModuleName, "FLTMGR.sys") == 0) {
		CHAR FullModuleName[100] = "C:\\windows\\system32\\drivers\\";
		lstrcatA(FullModuleName, ModuleName);// ƴ������·��
		// ����ģ�飬��������������  DONT_RESOLVE_DLL_REFERENCES:����ֻ�Ὣģ��ӳ�䵽��ǰ���̵ĵ�ַ�ռ䣬������������������ִ������ڵ�
		ntos = LoadLibraryExA(FullModuleName, NULL, DONT_RESOLVE_DLL_REFERENCES);
	}
	else {
		ntos = LoadLibraryA(ModuleName);
	}
	if (ntos == NULL) return 0;
	// ��ȡ�������û�̬�ĵ�ַ
	VOID* PocAddress = (VOID*)GetProcAddress(ntos, FuncName);
	// ���㺯�����û�̬ģ���е����ƫ��
	INT64 Offset = (INT64)PocAddress - (INT64)ntos;
	return (INT64)KBase + Offset;
}

VOID GenerateRandomName() {
	srand((UINT)time(NULL));

	INT length = rand() % 4 + 7;
	TCHAR charset[] = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	RandomName = (TCHAR*)calloc(length * 2 + 12, 1);
	if (RandomName) {
		for (INT i = 0; i < length; ++i) {
			INT index = rand() % (INT)(lstrlen(charset) - 1);
			RandomName[i] = charset[index];
		}
	}
	else {
		printf("Random Error!\n");
		ExitProcess(0);
	}
}
DWORD get_process_id(LPCTSTR process_name)
{
	PROCESSENTRY32 pt{};
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pt.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hsnap, &pt))
	{
		do {
			if (!lstrcmpi(pt.szExeFile, process_name))
			{
				CloseHandle(hsnap);
				return pt.th32ProcessID;
			}
		} while (Process32Next(hsnap, &pt));
	}
	CloseHandle(hsnap);
	return 0;
}
DWORD_PTR GetModuleBaseAddress(DWORD processId, const TCHAR* moduleName) {
	DWORD_PTR moduleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

	if (hSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(MODULEENTRY32);

		if (Module32First(hSnapshot, &moduleEntry)) {
			do {
				if (_tcsicmp(moduleEntry.szModule, moduleName) == 0) {
					moduleBaseAddress = (DWORD_PTR)moduleEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnapshot, &moduleEntry));
		}
	}

	CloseHandle(hSnapshot);
	return moduleBaseAddress;
}

int main(int argc, char* argv[])
{

	//if (argc != 2) {
	//	printf("Usage: RealBlindingEDR.exe [driver_path] [driver_type]\n\neg: RealBlindingEDR.exe c:\\echo_driver.sys \n");
	//	return 0;
	//}
	//DrivePath = argv[1];



	DrivePath = (CHAR*)"C:\\Users\\20629\\Desktop\\echo_driver.sys";

	GenerateRandomName();
	HINSTANCE hinst = LoadLibraryA("ntdll.dll");
	if (hinst == NULL) return FALSE;

	if (!InitialDriver()) return 0;

	DWORD pid = get_process_id(L"Bandizip.exe");
	if (pid == 0) {
		std::cout << "Failed to find Bandizip.exe process.\n";
		return -1;
	}
	std::cout << "Found Bandizip.exe with PID: " << pid << "\n";


	uintptr_t base_address = GetModuleBaseAddress(pid, L"Bandizip.exe");
	std::cout << "Base address of Bandizip.exe: 0x" << std::hex << base_address << std::dec << "\n";


	//Bandizip.exe+1F4A2B - CC                    - int 3 
	// ���� Bandizip.exe+1F4A2B �ĵ�ַ
	uintptr_t target_address = base_address + 0x1F4A2B;
	std::cout << "Target address (Bandizip.exe + 0x1F4A2B): 0x" << std::hex << target_address << std::dec << "\n";


	system("pause");

	UnloadDrive();
	return 0;
}

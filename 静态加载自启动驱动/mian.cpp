#define _CRT_SECURE_NO_WARNINGS

#include<windows.h>
#include<stdio.h>
#include <time.h>
#include <iostream>
#include <tchar.h>
#include "ntdll.h"
#pragma comment(lib,"ntdll.lib")
using namespace std;

#define NT_MACHINE                         L"\\Registry\\Machine\\"
#define SVC_BASE                           NT_MACHINE L"System\\CurrentControlSet\\Services\\"



#define DRIVER_FILE_NAME L"\\1.sys"  // 驱动名-----------------------------------------------------------------------------------------
static WCHAR LoaderServiceName[MAX_PATH];//服务名

static void FileNameToServiceName(PWCHAR ServiceName, PWCHAR FileName)
{
	// 将文件名转换为服务名，去除路径和扩展名，只保留文件名的核心部分
	int p = sizeof(SVC_BASE) / sizeof(WCHAR) - 1;
	wcscpy_s(ServiceName, sizeof(SVC_BASE) / sizeof(WCHAR), SVC_BASE);
	for (PWCHAR i = FileName; *i; ++i)
	{
		if (*i == L'\\')
			FileName = i + 1;
	}
	while (*FileName != L'\0' && *FileName != L'.')
		ServiceName[p++] = *FileName++;
	ServiceName[p] = L'\0';
}


static int ConvertToNtPath(PWCHAR Dst, PWCHAR Src)
{
	wcscpy_s(Dst, sizeof(L"\\??\\") / sizeof(WCHAR), L"\\??\\");
	wcscat_s(Dst, (MAX_PATH + sizeof(L"\\??\\")) / sizeof(WCHAR), Src);
	return static_cast<int>(wcslen(Dst)) * sizeof(wchar_t) + sizeof(wchar_t);
}

static NTSTATUS CreateDriverService(PWCHAR ServiceName, PWCHAR FileName)
{
	FileNameToServiceName(ServiceName, FileName);
	wprintf(L"FileNameToServiceName转换而来的ServiceName: %s\n", ServiceName);


	NTSTATUS Status = RtlCreateRegistryKey(0, ServiceName);
	if (!NT_SUCCESS(Status))
		return Status;

	WCHAR NtPath[MAX_PATH];
	ULONG ServiceType = SERVICE_KERNEL_DRIVER;

	Status = RtlWriteRegistryValue(RTL_REGISTRY_ABSOLUTE,
		ServiceName,
		L"ImagePath",
		REG_SZ,
		NtPath,
		ConvertToNtPath(NtPath, FileName));
	if (!NT_SUCCESS(Status))
		return Status;

	Status = RtlWriteRegistryValue(RTL_REGISTRY_ABSOLUTE,
		ServiceName,
		L"Type",
		REG_DWORD,
		&ServiceType,
		sizeof(ServiceType));

	// 设置服务启动类型为按需启动 (Demand Start)
	ULONG StartType = 1;
	Status = RtlWriteRegistryValue(RTL_REGISTRY_ABSOLUTE,
		ServiceName,
		L"Start",
		REG_DWORD,
		&StartType,
		sizeof(StartType));
	if (!NT_SUCCESS(Status))
		return Status;

	return Status;
}

static NTSTATUS LoadDriver(PWCHAR ServiceName)
{
	UNICODE_STRING ServiceNameUcs;
	RtlInitUnicodeString(&ServiceNameUcs, ServiceName);
	return NtLoadDriver(&ServiceNameUcs);
}

static NTSTATUS UnloadDriver(PWCHAR ServiceName)
{
	UNICODE_STRING ServiceNameUcs;
	RtlInitUnicodeString(&ServiceNameUcs, ServiceName);
	return NtUnloadDriver(&ServiceNameUcs);
}

BOOL CopyDriverToSystem32(const WCHAR* sourceFilePath) {
	// 使用宏定义的文件名作为目标路径
	WCHAR destPath[MAX_PATH];
	swprintf(destPath, MAX_PATH, L"C:\\Windows\\System32\\Drivers\\%s", DRIVER_FILE_NAME);

	// 强制复制文件，覆盖目标路径下的文件
	if (!CopyFileW(sourceFilePath, destPath, FALSE)) {
		wprintf(L"Failed to copy driver to System32\\Drivers\\: %d\n", GetLastError());
		return FALSE;
	}

	wprintf(L"Driver copied successfully to %s\n", destPath);
	return TRUE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//int main()
{

	//// 当前目录下的驱动文件路径
	wchar_t DriverPath[MAX_PATH];
	if (GetCurrentDirectoryW(MAX_PATH, DriverPath) == 0) {
		printf("Failed to get current directory.\n");
		return 1;
	}
	// 拼接当前目录下的 DRIVER_FILE_NAME 文件路径
	wcscat(DriverPath, DRIVER_FILE_NAME);


	//// 复制驱动文件到Windows/System32/Driver/路径
	if (CopyDriverToSystem32(DriverPath)) {
		DeleteFileW(DriverPath);// 删除当前路径下的 R2S1.sys 文件
	}
	else {
		printf("复制失败.\n");
		return 1;
	}


	// 将 DriverPath 更新为目标路径
	wcscpy(DriverPath, L"C:\\Windows\\System32\\Drivers");
	wcscat(DriverPath, DRIVER_FILE_NAME);

	// 打印启动路径
	wprintf(L"Driver is now at: %s\n", DriverPath);



	// Enable privileges
	constexpr const ULONG SE_LOAD_DRIVER_PRIVILEGE = 10UL;
	BOOLEAN SeLoadDriverWasEnabled;
	NTSTATUS Status = RtlAdjustPrivilege(SE_LOAD_DRIVER_PRIVILEGE,
		TRUE,
		FALSE,
		&SeLoadDriverWasEnabled);
	if (!NT_SUCCESS(Status))
	{
		printf("Fatal error: failed to acquire SE_LOAD_DRIVER_PRIVILEGE. Make sure you are running as administrator.\n");
		return Status;
	}

	WCHAR LoaderPath[MAX_PATH];
	// Expand filenames to full paths
	Status = RtlGetFullPathName_UEx((PWSTR)DriverPath, MAX_PATH * sizeof(WCHAR), LoaderPath, nullptr, nullptr);
	if (!NT_SUCCESS(Status))
		return Status;
	wprintf(L"RtlGetFullPathName_UEx转换而来的LoaderPath: %s\n", LoaderPath);

	Status = CreateDriverService(LoaderServiceName, LoaderPath); // 得到服务名
	if (!NT_SUCCESS(Status))
		return Status;

	// 传入拼接后的路径加载驱动
	Status = LoadDriver(LoaderServiceName);
	if (!NT_SUCCESS(Status)) {  // 如果加载驱动失败，打印错误并返回
		printf("Failed to load driver service %ls. NtLoadDriver: %08X.\n", LoaderServiceName, Status);
		return Status;
	}

	//system("pause");
	return 0;
}

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



#define DRIVER_FILE_NAME L"\\1.sys"  // ������-----------------------------------------------------------------------------------------
static WCHAR LoaderServiceName[MAX_PATH];//������

static void FileNameToServiceName(PWCHAR ServiceName, PWCHAR FileName)
{
	// ���ļ���ת��Ϊ��������ȥ��·������չ����ֻ�����ļ����ĺ��Ĳ���
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
	wprintf(L"FileNameToServiceNameת��������ServiceName: %s\n", ServiceName);


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

	// ���÷�����������Ϊ�������� (Demand Start)
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
	// ʹ�ú궨����ļ�����ΪĿ��·��
	WCHAR destPath[MAX_PATH];
	swprintf(destPath, MAX_PATH, L"C:\\Windows\\System32\\Drivers\\%s", DRIVER_FILE_NAME);

	// ǿ�Ƹ����ļ�������Ŀ��·���µ��ļ�
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

	//// ��ǰĿ¼�µ������ļ�·��
	wchar_t DriverPath[MAX_PATH];
	if (GetCurrentDirectoryW(MAX_PATH, DriverPath) == 0) {
		printf("Failed to get current directory.\n");
		return 1;
	}
	// ƴ�ӵ�ǰĿ¼�µ� DRIVER_FILE_NAME �ļ�·��
	wcscat(DriverPath, DRIVER_FILE_NAME);


	//// ���������ļ���Windows/System32/Driver/·��
	if (CopyDriverToSystem32(DriverPath)) {
		DeleteFileW(DriverPath);// ɾ����ǰ·���µ� R2S1.sys �ļ�
	}
	else {
		printf("����ʧ��.\n");
		return 1;
	}


	// �� DriverPath ����ΪĿ��·��
	wcscpy(DriverPath, L"C:\\Windows\\System32\\Drivers");
	wcscat(DriverPath, DRIVER_FILE_NAME);

	// ��ӡ����·��
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
	wprintf(L"RtlGetFullPathName_UExת��������LoaderPath: %s\n", LoaderPath);

	Status = CreateDriverService(LoaderServiceName, LoaderPath); // �õ�������
	if (!NT_SUCCESS(Status))
		return Status;

	// ����ƴ�Ӻ��·����������
	Status = LoadDriver(LoaderServiceName);
	if (!NT_SUCCESS(Status)) {  // �����������ʧ�ܣ���ӡ���󲢷���
		printf("Failed to load driver service %ls. NtLoadDriver: %08X.\n", LoaderServiceName, Status);
		return Status;
	}

	//system("pause");
	return 0;
}

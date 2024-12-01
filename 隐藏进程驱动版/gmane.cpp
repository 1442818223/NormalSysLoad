#include "gmane.h"
#include "LoadDriver.h"
#include "api.h"
#include <iostream>

typedef struct _CommInfo {
	ULONG64 inData;
} CommInfo, * PCommInfo;


// ���豸
HANDLE OpenDevice(LPCWSTR devicePath) {
	HANDLE hDevice = CreateFile(
		devicePath,                     // �豸·��
		GENERIC_WRITE | GENERIC_READ,    // ��дȨ��
		FILE_SHARE_READ | FILE_SHARE_WRITE, // �����д
		NULL,                         // ��ȫ����
		OPEN_EXISTING,                // �������ļ�
		0,                            // �ļ�����
		NULL                          // ģ���ļ�
	);

	if (hDevice == INVALID_HANDLE_VALUE) {
		// ��ʧ�ܣ����������Ϣ
		printf("��ʧ��\n");
	}

	return hDevice;
}
// ����豸�Ƿ��Ѽ���
BOOL IsDriverLoaded() {
	HANDLE hDevice = OpenDevice(L"\\\\.\\MMMMMu");
	if (hDevice != INVALID_HANDLE_VALUE) {
		CloseHandle(hDevice);
		return TRUE; // �豸�Ѽ���
	}
	return FALSE; // �豸δ����
}
// ���� IOCTL ����
BOOL SendIoctl(HANDLE hDevice, DWORD ioctlCode, CommInfo* ����) {
	DWORD returned = 0;

	BOOL success = DeviceIoControl(
		hDevice,
		ioctlCode,
		����,             // ���뻺����
		sizeof(CommInfo), // ���뻺������С
		����,             // ���������
		sizeof(CommInfo), // �����������С
		&returned,        // ���ص��ֽ���
		nullptr           // �ص��ṹ
	);

	if (success) {
		std::cout << "Successfully sent IOCTL code: " << ioctlCode << std::endl;
	}
	else {
		std::cout << "Failed to send IOCTL code: " << ioctlCode << ". Error: " << GetLastError() << std::endl;
	}

	return success;
}

 
 BOOL LOADN()
 {
	 HANDLE hDevice = NULL;

	 // ��������Ƿ��Ѽ���
	 if (!IsDriverLoaded()) {
		 printf("����δ���أ����ڼ���...\n");

		 // �������δ���أ����¼�������
		 if (!SH_DriverLoad()) {
			 return FALSE; // ��������ʧ��
		 }
	 }

	 // ����������ͨ�Ų���
	  hDevice = OpenDevice(L"\\\\.\\MMMMMu");
	 if (hDevice == INVALID_HANDLE_VALUE) {
		 std::cerr << "Failed to open device. Error: " << GetLastError() << std::endl;
		 return FALSE;
	 }
	
	 // ���Ͳ��� IOCTL ����
	 CommInfo info;
	 info.inData = 0x9999999;
	 BOOL success = SendIoctl(hDevice, ioctl_TEST, &info);
	 if (success && info.inData == 0x1111111) {
		 return TRUE;
	 }
	 CloseHandle(hDevice);

	 return  FALSE;
 }


// ��������
  BOOLEAN   LOADENTRY(UINT pid) {

	if(!LOADN()) return FALSE ;

	HANDLE hDevice = OpenDevice(L"\\\\.\\MMMMMu");

	CommInfo info;
	info.inData = pid;
	BOOL ison = SendIoctl(hDevice, ioctl_PRETEND, &info);
	if (ison)
	{ // �� PID ת��Ϊ�ַ�������ӵ���Ϣ��������
		char message[256];
		snprintf(message, sizeof(message), "���ص�Ŀ��PID: %u �ɹ�", pid);

		MessageBoxA(NULL, message, "��ʾ", MB_OK);
	}

	CloseHandle(hDevice); // ȷ���ڽ���ǰ�ر��豸���

	return ison;
}

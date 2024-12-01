#include "gmane.h"
#include "LoadDriver.h"
#include "api.h"
#include <iostream>

typedef struct _CommInfo {
	ULONG64 inData;
} CommInfo, * PCommInfo;


// 打开设备
HANDLE OpenDevice(LPCWSTR devicePath) {
	HANDLE hDevice = CreateFile(
		devicePath,                     // 设备路径
		GENERIC_WRITE | GENERIC_READ,    // 读写权限
		FILE_SHARE_READ | FILE_SHARE_WRITE, // 共享读写
		NULL,                         // 安全属性
		OPEN_EXISTING,                // 打开现有文件
		0,                            // 文件属性
		NULL                          // 模板文件
	);

	if (hDevice == INVALID_HANDLE_VALUE) {
		// 打开失败，输出错误信息
		printf("打开失败\n");
	}

	return hDevice;
}
// 检查设备是否已加载
BOOL IsDriverLoaded() {
	HANDLE hDevice = OpenDevice(L"\\\\.\\MMMMMu");
	if (hDevice != INVALID_HANDLE_VALUE) {
		CloseHandle(hDevice);
		return TRUE; // 设备已加载
	}
	return FALSE; // 设备未加载
}
// 发送 IOCTL 请求
BOOL SendIoctl(HANDLE hDevice, DWORD ioctlCode, CommInfo* 数据) {
	DWORD returned = 0;

	BOOL success = DeviceIoControl(
		hDevice,
		ioctlCode,
		数据,             // 输入缓冲区
		sizeof(CommInfo), // 输入缓冲区大小
		数据,             // 输出缓冲区
		sizeof(CommInfo), // 输出缓冲区大小
		&returned,        // 返回的字节数
		nullptr           // 重叠结构
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

	 // 检查驱动是否已加载
	 if (!IsDriverLoaded()) {
		 printf("驱动未加载，正在加载...\n");

		 // 如果驱动未加载，重新加载驱动
		 if (!SH_DriverLoad()) {
			 return FALSE; // 加载驱动失败
		 }
	 }

	 // 尝试与驱动通信测试
	  hDevice = OpenDevice(L"\\\\.\\MMMMMu");
	 if (hDevice == INVALID_HANDLE_VALUE) {
		 std::cerr << "Failed to open device. Error: " << GetLastError() << std::endl;
		 return FALSE;
	 }
	
	 // 发送测试 IOCTL 请求
	 CommInfo info;
	 info.inData = 0x9999999;
	 BOOL success = SendIoctl(hDevice, ioctl_TEST, &info);
	 if (success && info.inData == 0x1111111) {
		 return TRUE;
	 }
	 CloseHandle(hDevice);

	 return  FALSE;
 }


// 启动函数
  BOOLEAN   LOADENTRY(UINT pid) {

	if(!LOADN()) return FALSE ;

	HANDLE hDevice = OpenDevice(L"\\\\.\\MMMMMu");

	CommInfo info;
	info.inData = pid;
	BOOL ison = SendIoctl(hDevice, ioctl_PRETEND, &info);
	if (ison)
	{ // 将 PID 转换为字符串并添加到消息框内容中
		char message[256];
		snprintf(message, sizeof(message), "隐藏的目标PID: %u 成功", pid);

		MessageBoxA(NULL, message, "提示", MB_OK);
	}

	CloseHandle(hDevice); // 确保在结束前关闭设备句柄

	return ison;
}

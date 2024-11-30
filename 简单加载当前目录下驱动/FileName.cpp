#include <Windows.h>
#include <iostream>
#include <string>


int main() {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 获取当前工作目录
	char currentPath[MAX_PATH];
	if (GetCurrentDirectoryA(MAX_PATH, currentPath) == 0) {
		printf("获取当前目录失败\n");
		return 1;
	}

	// 构造驱动程序的完整路径
	char driverPath[MAX_PATH];
	snprintf(driverPath, sizeof(driverPath), "%s\\CbsTemp.sys", currentPath); // 假设驱动程序文件名为 12356.sys
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 停止服务的命令   CbsTemp这两个都是服务名
	const char* stopCmd = "sc stop CbsTemp";  // 修改为 MyDriver
	// 删除服务的命令
	const char* deleteCmd = "sc delete CbsTemp"; // 修改为 MyDriver

	// 执行停止服务命令
	int resultStop = system(stopCmd);
	if (resultStop != 0) {
		printf("停止服务失败\n");
	}

	// 执行删除服务命令
	int resultDelete = system(deleteCmd);
	if (resultDelete != 0) {
		printf("删除服务失败\n");
	}




/*
将这两步添加到此c语言程序中

rem 复制12356.sys到C:\Windows\System32\drivers下面
copy "%~dp012356.sys" "C:\Windows\System32\drivers\" >nul
if %errorlevel% neq 0 (
	echo 复制驱动文件失败，请检查文件路径和权限。
	exit /b
)

rem 删除当前目录下的12356.sys
del "%~dp012356.sys" >nul
if %errorlevel% neq 0 (
	echo 删除当前目录下的驱动文件失败，请检查权限。
	exit /b
)

下面的驱动启动路径也按C:\Windows\System32\drivers来进行

*/

// 复制驱动文件到 C:\Windows\System32\drivers
	char copyCmd[512];
	snprintf(copyCmd, sizeof(copyCmd), "xcopy /Y \"%s\" \"C:\\Windows\\System32\\drivers\\\" >nul", driverPath);
	int resultCopy = system(copyCmd);
	if (resultCopy != 0) {
		printf("复制驱动文件失败，请检查文件路径和权限。\n");
		return 1;
	}
	// 删除当前目录下的驱动文件命令
	char deleteCurrentCmd[512];
	snprintf(deleteCurrentCmd, sizeof(deleteCurrentCmd), "del \"%s\" >nul", driverPath);
	int resultDeleteCurrent = system(deleteCurrentCmd);
	if (resultDeleteCurrent != 0) {
		printf("删除当前目录下的驱动文件失败，请检查权限。\n");
		return 1;
	}


	// 创建服务命令     CbsTemp这两个都是服务名
	char createCmd[512];
	snprintf(createCmd, sizeof(createCmd), "sc create CbsTemp binPath= \"C:\\Windows\\System32\\drivers\\CbsTemp.sys\" type= kernel start= demand", driverPath);
	int resultCreate = system(createCmd);
	if (resultCreate != 0) {
		printf("创建服务失败\n");
		return resultCreate;
	}





	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// 创建服务命令     CbsTemp这两个都是服务名
	//char createCmd[512];
	//snprintf(createCmd, sizeof(createCmd), "sc create CbsTemp binPath= \"%s\" type= kernel start= demand", driverPath);
	//int resultCreate = system(createCmd);
	//if (resultCreate != 0) {
	//	printf("创建服务失败\n");
	//	return resultCreate;
	//}

	// 启动服务命令
	const char* startCmd = "sc start CbsTemp";
	int resultStart = system(startCmd);
	if (resultStart != 0) {
		printf("启动服务失败\n");
		return resultStart;
	}

	printf("驱动程序已成功加载并启动。\n");

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	HKEY hKey;
	LONG result;

	// 打开服务的注册表项
	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		TEXT("SYSTEM\\CurrentControlSet\\Services\\CbsTemp"),
		0,
		KEY_SET_VALUE,
		&hKey);

	if (result != ERROR_SUCCESS) {
		printf("无法打开注册表项，错误代码：%ld\n", result);
		return 1;
	}

	// 设置 Start 值为 1
	DWORD startValue = 1; // 1 表示自动启动
	result = RegSetValueEx(hKey,
		TEXT("Start"),
		0,
		REG_DWORD,
		(const BYTE*)&startValue,
		sizeof(startValue));

	if (result != ERROR_SUCCESS) {
		printf("无法设置 Start 值，错误代码：%ld\n", result);
		RegCloseKey(hKey);
		return 1;
	}

	printf("驱动自启动已开启。\n");

	// 关闭注册表项
	RegCloseKey(hKey);

	system("pause");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
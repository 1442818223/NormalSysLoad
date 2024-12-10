#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <cstdio>
#include <cstdlib>

// Function to request Administrator permissions
void RunAsAdministrator(const std::string& scriptPath) {
	std::string vbsFile = std::tmpnam(nullptr);
	vbsFile += ".vbs";
	std::ofstream vbsScript(vbsFile);

	if (vbsScript.is_open()) {
		vbsScript << "Set UAC = CreateObject(\"Shell.Application\")\n";
		vbsScript << "UAC.ShellExecute \"" << scriptPath << "\", \"\", \"\", \"runas\", 1\n";
		vbsScript.close();

		// Execute the VBS script
		system(("cscript //nologo " + vbsFile).c_str());
		// Delete the temporary VBS script
		std::remove(vbsFile.c_str());
	}
}

// Function to execute a WMIC command and capture the output
std::string GetWmicInfo(const std::string& wmicCommand) {
	char buffer[128];
	std::string result = "";
	FILE* pipe = _popen(wmicCommand.c_str(), "r");
	if (!pipe) return "ERROR";

	while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
		result += buffer;
	}
	_pclose(pipe);
	return result;
}

// Function to execute a Powershell command and capture the output
std::string GetPowershellInfo(const std::string& psCommand) {
	char buffer[128];
	std::string result = "";
	FILE* pipe = _popen(("powershell -Command \"" + psCommand + "\"").c_str(), "r");
	if (!pipe) return "ERROR";

	while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
		result += buffer;
	}
	_pclose(pipe);
	return result;
}

// Function to get system information (BIOS, CPU, etc.)
void GetSystemInfo() {
	std::cout << "机器码大大专用硬件检测（TP/EAC/BE/私服-台式机/笔记本/一体机）" << std::endl;
	std::cout << "____________________________大大硬件检测专业解机器码QQ6953271____________________________" << std::endl;

	// Get system manufacturer and baseboard product
	std::string brand = GetWmicInfo("wmic bios get manufacturer");
	std::string model1 = GetWmicInfo("wmic baseboard get product");
	std::string model2 = GetWmicInfo("wmic baseboard get manufacturer");

	std::cout << "品牌: " << brand << std::endl;
	std::cout << "型号: " << model1 << std::endl;
	std::cout << "型号: " << model2 << std::endl;

	// Get CPU serial number
	std::cout << "CPU序列号: " << GetWmicInfo("wmic cpu get serialnumber") << std::endl;

	// Get motherboard serial number and UUID
	std::cout << "主板序列号: " << GetWmicInfo("wmic baseboard get serialnumber") << std::endl;
	std::cout << "主板UUID: " << GetWmicInfo("wmic csproduct get UUID") << std::endl;

	// Get memory serial number
	std::cout << "内存条序列号: " << GetWmicInfo("wmic memorychip get serialnumber") << std::endl;

	// Get computer name
	std::cout << "计算机名: " << GetWmicInfo("hostname") << std::endl;

	// Get disk serial number
	std::cout << "硬盘物理序列号: " << GetWmicInfo("wmic diskdrive get serialnumber") << std::endl;
}

// Function to get TPM 2.0 status
std::string GetTpmStatus() {
	return GetPowershellInfo("get-tpm | findstr /i \"TpmPresent\"");
}

// Function to get Secure Boot status
std::string GetSecureBootStatus() {
	return GetPowershellInfo("Confirm-SecureBootUEFI");
}

// Function to get initial installation date and last boot time
void GetSystemTimeInfo() {
	std::string installDate = GetPowershellInfo("$os = Get-WmiObject -Class Win32_OperatingSystem; $installDate = $os.InstallDate; if ($installDate) { $installDate = [System.Management.ManagementDateTimeConverter]::ToDateTime($installDate); Write-Output $installDate.ToString('yyyy-MM-dd') }");
	std::string lastBootUpTime = GetPowershellInfo("$os = Get-WmiObject -Class Win32_OperatingSystem; $lastBootUpTime = $os.LastBootUpTime; if ($lastBootUpTime) { $lastBootUpTime = [System.Management.ManagementDateTimeConverter]::ToDateTime($lastBootUpTime); Write-Output $lastBootUpTime.ToString('yyyy-MM-dd HH:mm:ss') }");

	std::cout << "初始安装日期: " << (installDate.empty() ? "无法获取" : installDate) << std::endl;
	std::cout << "系统启动时间: " << (lastBootUpTime.empty() ? "无法获取" : lastBootUpTime) << std::endl;
}

// Function to get network information
void GetNetworkInfo() {
	std::cout << "默认网关: " << GetWmicInfo("route print | findstr \"<0.0.0.0>\"") << std::endl;
	std::cout << "网卡信息 MAC: " << GetWmicInfo("ipconfig /all | findstr /i \"描述 物理地址\"") << std::endl;
	std::cout << "DUID: " << GetWmicInfo("ipconfig /all | findstr /i \"DUID\"") << std::endl;
}

// Main function to integrate all actions
int main() {
	// Check if we have administrator privileges
	if (system("cacls \"%SYSTEMROOT%\\system32\\config\\system\" >nul 2>&1") != 0) {
		// If not, elevate privileges
		RunAsAdministrator("your_script_path_here.bat");
		return 0;
	}

	// Get system information
	GetSystemInfo();

	// Get TPM and Secure Boot status
	std::string tpmStatus = GetTpmStatus();
	std::string secureBootStatus = GetSecureBootStatus();
	std::cout << "TPM 2.0: " << tpmStatus << "   安全启动: " << secureBootStatus << std::endl;

	// Get system time information
	GetSystemTimeInfo();

	// Get network information
	GetNetworkInfo();

	// Query public IP address
	std::cout << "获取公网IP地址..." << std::endl;
	system("curl https://myip.ipip.net");

	std::cout << "____________________________大大硬件检测专业解机器码QQ6953271____________________________" << std::endl;
	std::cout << "Press Enter to Update" << std::endl;
	std::cin.get();

	// 提示用户继续操作
	system("cls");
	system("bcdedit >nul");
	if (system("echo %errorlevel%") != 0) {
		std::cout << "需要管理员权限" << std::endl;
		system("start mshta vbscript:createobject(\"shell.application\").shellexecute(\"\"\"%~0\"\"\",\"::\",,\"runas\",0)(window.close)&exit");
		return 0;
	}
	else {
		system("net stop winmgmt /y");
		system("taskkill /F /IM WmiPrvSE.exe");
	}

	// 结束程序
	std::cout << "查询完毕,按回车结束。" << std::endl;
	std::cin.get();



	system("pause");
	return 0;
}

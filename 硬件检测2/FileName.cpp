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
	std::cout << "��������ר��Ӳ����⣨TP/EAC/BE/˽��-̨ʽ��/�ʼǱ�/һ�����" << std::endl;
	std::cout << "____________________________���Ӳ�����רҵ�������QQ6953271____________________________" << std::endl;

	// Get system manufacturer and baseboard product
	std::string brand = GetWmicInfo("wmic bios get manufacturer");
	std::string model1 = GetWmicInfo("wmic baseboard get product");
	std::string model2 = GetWmicInfo("wmic baseboard get manufacturer");

	std::cout << "Ʒ��: " << brand << std::endl;
	std::cout << "�ͺ�: " << model1 << std::endl;
	std::cout << "�ͺ�: " << model2 << std::endl;

	// Get CPU serial number
	std::cout << "CPU���к�: " << GetWmicInfo("wmic cpu get serialnumber") << std::endl;

	// Get motherboard serial number and UUID
	std::cout << "�������к�: " << GetWmicInfo("wmic baseboard get serialnumber") << std::endl;
	std::cout << "����UUID: " << GetWmicInfo("wmic csproduct get UUID") << std::endl;

	// Get memory serial number
	std::cout << "�ڴ������к�: " << GetWmicInfo("wmic memorychip get serialnumber") << std::endl;

	// Get computer name
	std::cout << "�������: " << GetWmicInfo("hostname") << std::endl;

	// Get disk serial number
	std::cout << "Ӳ���������к�: " << GetWmicInfo("wmic diskdrive get serialnumber") << std::endl;
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

	std::cout << "��ʼ��װ����: " << (installDate.empty() ? "�޷���ȡ" : installDate) << std::endl;
	std::cout << "ϵͳ����ʱ��: " << (lastBootUpTime.empty() ? "�޷���ȡ" : lastBootUpTime) << std::endl;
}

// Function to get network information
void GetNetworkInfo() {
	std::cout << "Ĭ������: " << GetWmicInfo("route print | findstr \"<0.0.0.0>\"") << std::endl;
	std::cout << "������Ϣ MAC: " << GetWmicInfo("ipconfig /all | findstr /i \"���� �����ַ\"") << std::endl;
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
	std::cout << "TPM 2.0: " << tpmStatus << "   ��ȫ����: " << secureBootStatus << std::endl;

	// Get system time information
	GetSystemTimeInfo();

	// Get network information
	GetNetworkInfo();

	// Query public IP address
	std::cout << "��ȡ����IP��ַ..." << std::endl;
	system("curl https://myip.ipip.net");

	std::cout << "____________________________���Ӳ�����רҵ�������QQ6953271____________________________" << std::endl;
	std::cout << "Press Enter to Update" << std::endl;
	std::cin.get();

	// ��ʾ�û���������
	system("cls");
	system("bcdedit >nul");
	if (system("echo %errorlevel%") != 0) {
		std::cout << "��Ҫ����ԱȨ��" << std::endl;
		system("start mshta vbscript:createobject(\"shell.application\").shellexecute(\"\"\"%~0\"\"\",\"::\",,\"runas\",0)(window.close)&exit");
		return 0;
	}
	else {
		system("net stop winmgmt /y");
		system("taskkill /F /IM WmiPrvSE.exe");
	}

	// ��������
	std::cout << "��ѯ���,���س�������" << std::endl;
	std::cin.get();



	system("pause");
	return 0;
}

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    DWORD dwByteWrite = 0;
    char bufPath[MAX_PATH] = { 0 };
	char batContent[] =
		"@echo off\r\n"
		">nul 2>&1 \"%SYSTEMROOT%\\system32\\cacls.exe\" \"%SYSTEMROOT%\\system32\\config\\system\"\r\n"
		"if '%errorlevel%' NEQ '0' (\r\n"
		"goto UACPrompt\r\n"
		") else ( goto gotAdmin )\r\n"
		":UACPrompt\r\n"
		"echo Set UAC = CreateObject^(\"Shell.Application\"^) > \"%temp%\\getadmin.vbs\"\r\n"
		"echo UAC.ShellExecute \"%~s0\", \"\", \"\", \"runas\", 1 >> \"%temp%\\getadmin.vbs\"\r\n"
		"\"%temp%\\getadmin.vbs\"\r\n"
		"exit /B\r\n"
		":gotAdmin\r\n"
		"if exist \"%temp%\\getadmin.vbs\" ( del \"%temp%\\getadmin.vbs\" )\r\n"
		"@echo off\r\n"
		"title 机器码大大专用硬件检测（TP/EAC/BE/私服-台式机/笔记本/一体机）\r\n"
		"==\r\n"
		"mode con cols=90 lines=150\r\n"
		"cls\r\n"
		"echo. \r\n"
		"echo.____________________________大大硬件检测专业解机器码QQ6953271____________________________\r\n"
		"echo.\r\n"
		"echo.    主版:\r\n"
		"for /f \"tokens=* delims=\" %%a in ('reg query \"HKLM\\HARDWARE\\DESCRIPTION\\System\\BIOS\" /v \"SystemManufacturer\"') do (set \"Brand=%%a\")\r\n"
		"for /f \"tokens=* delims=\" %%a in ('reg query \"HKLM\\HARDWARE\\DESCRIPTION\\System\\BIOS\" /v \"BaseBoardProduct\"') do (set \"model1=%%a\")\r\n"
		"set \"model2=\"&for /f \"skip=1 tokens=*\" %%m in ('wmic baseboard get manufacturer') do if not defined model2 set \"model2=%%m\"\r\n"
		"echo.            品牌：%Brand:~36%\r\n"
		"echo.            型号：%model1:~34%\r\n"
		"echo.            型号：%model2%\r\n"
		"echo.\r\n"
		"echo.名字\r\n"
		"wmic csproduct get Name\r\n"
		"echo.硬盘物理序列号\r\n"
		"wmic diskdrive get serialnumber\r\n"
		"echo.CPU序列号:\r\n"
		"wmic cpu get serialnumber\r\n"
		"echo.主板biso序列号\r\n"
		"wmic bios get serialnumber\r\n"
		"echo.主板物理序列号\r\n"
		"wmic baseboard get serialnumber\r\n"
		"echo.主板UUID\r\n"
		"wmic csproduct get UUID\r\n"
		"@echo off\r\n"
		"setlocal\r\n"
		":: 获取CPU的processorid，并去除多余的空行和空格\r\n"
		"for /f \"skip=1 tokens=2 delims== \" %%i in ('wmic cpu get processorid /format:list') do (\r\n"
		"    set \"cpu_id=%%i\"\r\n"
		"    goto :break_cpu\r\n"
		")\r\n"
		":break_cpu\r\n"
		":: 获取内存条的serialnumber，假设只有一条内存条，并去除多余的空行和空格\r\n"
		"set \"mem_serial=\"\r\n"
		"for /f \"skip=1 tokens=2 delims== \" %%j in ('wmic memorychip get serialnumber /format:list') do (\r\n"
		"    set \"mem_serial=%%j\"\r\n"
		"    goto :break_mem\r\n"
		")\r\n"
		":break_mem\r\n"
		":: 获取计算机名\r\n"
		"for /f \"tokens=*\" %%k in ('hostname') do set \"computer_name=%%k\"\r\n"
		":: 合并输出\r\n"
		"echo CPU: %cpu_id%  内存条: %mem_serial%  计算机名: %computer_name%\r\n"
		"endlocal\r\n"
		"echo [90m---------------------------------------------------------------------------------------[98m\r\n"
		"echo.默认网关\r\n"
		"for /f \"tokens=3 delims=: \" %%G in ('route print ^| findstr \"\<0.0.0.0\>\"') do (\r\n"
		"    echo %%G\r\n"
		")\r\n"
		"echo [90m---------------------------------------------------------------------------------------[98m\r\n"
		"echo.网卡信息MAC\r\n"
		"@echo off&color 0A&&setlocal EnableDelayedExpansion\r\n"
		"for /f \"tokens=*\" %%i in ('ipconfig /all^|findstr /i \"描述 物理地址\"') do set \"qq=%%i\"&&set \"qq=!qq:. =!\"&&echo.!qq!\r\n"
		"echo.\r\n"
		"for /f \"tokens=*\" %%i in ('ipconfig /all^|findstr /i \"DUID\"') do set \"qq=%%i\"&&set \"qq=!qq:. =!\"&&echo.!qq!\r\n"
		"echo.\r\n"
		"wmic path Win32_NetworkAdapter where \"PNPDeviceID like '%%PCI%%' AND NetConnectionStatus=2 AND AdapterTypeID='0'\" get MacAddress\r\n"
		"@echo off\r\n"
		":: 查询初始安装日期\r\n"
		"for /f \"delims=\" %%i in ('powershell.exe -Command \"$os = Get-WmiObject -Class Win32_OperatingSystem; $installDate = $os.InstallDate; if ($installDate) { $installDate = [System.Management.ManagementDateTimeConverter]::ToDateTime($installDate); Write-Output $installDate.ToString('yyyy-MM-dd') }\"') do (\r\n"
		"    set installDate=%%i\r\n"
		")\r\n"
		"if defined installDate (\r\n"
		"    echo 初始安装日期: %installDate%\r\n"
		") else (\r\n"
		"    echo 无法获取初始安装日期。\r\n"
		")\r\n"
		"echo.\r\n"
		":: 查询系统上次启动时间\r\n"
		"for /f \"delims=\" %%i in ('powershell.exe -Command \"$os = Get-WmiObject -Class Win32_OperatingSystem; $lastBootUpTime = $os.LastBootUpTime; if ($lastBootUpTime) { $lastBootUpTime = [System.Management.ManagementDateTimeConverter]::ToDateTime($lastBootUpTime); Write-Output $lastBootUpTime.ToString('yyyy-MM-dd HH:mm:ss') }\"') do (\r\n"
		"    set lastBootUpTime=%%i\r\n"
		")\r\n"
		"if defined lastBootUpTime (\r\n"
		"    echo 系统启动时间: %lastBootUpTime%\r\n"
		") else (\r\n"
		"    echo 无法获取系统上次启动时间。\r\n"
		")\r\n"
		"echo.\r\n"
		"@echo off\r\n"
		":: 获取TPM 2.0状态\r\n"
		"for /f \"tokens=3\" %%i in ('powershell get-tpm^|findstr /i \"TpmPresent \"') do (set TpmPresent=%%i)\r\n"
		":: 获取Secure Boot状态\r\n"
		"set \"SecureBootStatus=False\"\r\n"
		"for /f %%i in ('powershell.exe -Command \"Confirm-SecureBootUEFI\"') do set \"SecureBootStatus=%%i\"\r\n"
		":: 输出TPM 2.0和Secure Boot状态在同一行\r\n"
		"echo TPM2.0: %TpmPresent%   安全启动: %SecureBootStatus%\r\n"
		"echo.\r\n"
		"curl https://myip.ipip.net\r\n"
		"echo.\r\n"
		"echo.____________________________大大硬件检测专业解机器码QQ6953271____________________________\r\n"
		"echo.\r\n"
		"echo Press Enter to Update\r\n"
		"pause>nul\r\n"
		"cls\r\n"
		"if exist \"%SystemRoot%\\SysWOW64\" path %path%;%windir%\\SysNative;%SystemRoot%\\SysWOW64;%~dp0\r\n"
		"bcdedit >nul\r\n"
		"if '%errorlevel%' NEQ '0' (goto UACPrompt) else (goto UACAdmin)\r\n"
		":UACPrompt\r\n"
		"%1 start \"\" mshta vbscript:createobject(\"shell.application\").shellexecute(\"\"\"%~0\"\"\",\"::\",\"runas\",0)(window.close)&exit\r\n"
		"exit /B\r\n"
		":UACAdmin\r\n"
		"net stop winmgmt /y\r\n"
		"taskkill /F /IM WmiPrvSE.exe\r\n"
		"goto check\r\n"
		"echo. _____________________________________________________________________________\r\n"
		"echo.\r\n"
		"echo.\r\n"
		"echo.查询完毕,按回车结束。\r\n"
		"pause>nul\r\n"
		"goto exit\r\n"
		"ECHO 处于关闭状态。\r\n";


    HANDLE hFile;
    // 获取临时路径并拼接文件名
    GetTempPathA(MAX_PATH, bufPath);
    snprintf(bufPath, MAX_PATH, "%s1K.bat", bufPath);

    // 打印调试信息，检查路径是否正确
    OutputDebugStringA(bufPath);

    // 创建文件并写入 .bat 内容
    hFile = CreateFileA(bufPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        DWORD err = GetLastError();
        char errMsg[128];
        snprintf(errMsg, sizeof(errMsg), "Failed to create 1K.bat file (error %lu)", err);
        OutputDebugStringA(errMsg);
        return 1;
    }

    // 写入 .bat 文件内容
    if (!WriteFile(hFile, batContent, strlen(batContent), &dwByteWrite, NULL)) {
        DWORD err = GetLastError();
        char errMsg[128];
        snprintf(errMsg, sizeof(errMsg), "WriteFile failed for 1K.bat (error %lu)", err);
        OutputDebugStringA(errMsg);
        CloseHandle(hFile);
        return 1;
    }

    // 检查写入的字节数
    if (dwByteWrite != strlen(batContent)) {
        char errMsg[128];
        snprintf(errMsg, sizeof(errMsg), "Byte write mismatch for 1K.bat. Expected %lu, wrote %lu.", strlen(batContent), dwByteWrite);
        OutputDebugStringA(errMsg);
        CloseHandle(hFile);
        return 1;
    }

    // 关闭文件
    CloseHandle(hFile);

    // 执行批处理文件
    printf("准备执行批处理文件: %s\n", bufPath);
    int result = system(bufPath);
    if (result != 0) {
        printf("执行批处理文件失败. 错误码: %d\n", result);
        return 1;
    }

    // 删除临时批处理文件
    DeleteFileA(bufPath);
    return 0;
}

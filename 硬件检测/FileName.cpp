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
		"title »úÆ÷Âë´ó´ó×¨ÓÃÓ²¼ş¼ì²â£¨TP/EAC/BE/Ë½·ş-Ì¨Ê½»ú/±Ê¼Ç±¾/Ò»Ìå»ú£©\r\n"
		"==\r\n"
		"mode con cols=90 lines=150\r\n"
		"cls\r\n"
		"echo. \r\n"
		"echo.____________________________´ó´óÓ²¼ş¼ì²â×¨Òµ½â»úÆ÷ÂëQQ6953271____________________________\r\n"
		"echo.\r\n"
		"echo.    Ö÷°æ:\r\n"
		"for /f \"tokens=* delims=\" %%a in ('reg query \"HKLM\\HARDWARE\\DESCRIPTION\\System\\BIOS\" /v \"SystemManufacturer\"') do (set \"Brand=%%a\")\r\n"
		"for /f \"tokens=* delims=\" %%a in ('reg query \"HKLM\\HARDWARE\\DESCRIPTION\\System\\BIOS\" /v \"BaseBoardProduct\"') do (set \"model1=%%a\")\r\n"
		"set \"model2=\"&for /f \"skip=1 tokens=*\" %%m in ('wmic baseboard get manufacturer') do if not defined model2 set \"model2=%%m\"\r\n"
		"echo.            Æ·ÅÆ£º%Brand:~36%\r\n"
		"echo.            ĞÍºÅ£º%model1:~34%\r\n"
		"echo.            ĞÍºÅ£º%model2%\r\n"
		"echo.\r\n"
		"echo.Ãû×Ö\r\n"
		"wmic csproduct get Name\r\n"
		"echo.Ó²ÅÌÎïÀíĞòÁĞºÅ\r\n"
		"wmic diskdrive get serialnumber\r\n"
		"echo.CPUĞòÁĞºÅ:\r\n"
		"wmic cpu get serialnumber\r\n"
		"echo.Ö÷°åbisoĞòÁĞºÅ\r\n"
		"wmic bios get serialnumber\r\n"
		"echo.Ö÷°åÎïÀíĞòÁĞºÅ\r\n"
		"wmic baseboard get serialnumber\r\n"
		"echo.Ö÷°åUUID\r\n"
		"wmic csproduct get UUID\r\n"
		"@echo off\r\n"
		"setlocal\r\n"
		":: »ñÈ¡CPUµÄprocessorid£¬²¢È¥³ı¶àÓàµÄ¿ÕĞĞºÍ¿Õ¸ñ\r\n"
		"for /f \"skip=1 tokens=2 delims== \" %%i in ('wmic cpu get processorid /format:list') do (\r\n"
		"    set \"cpu_id=%%i\"\r\n"
		"    goto :break_cpu\r\n"
		")\r\n"
		":break_cpu\r\n"
		":: »ñÈ¡ÄÚ´æÌõµÄserialnumber£¬¼ÙÉèÖ»ÓĞÒ»ÌõÄÚ´æÌõ£¬²¢È¥³ı¶àÓàµÄ¿ÕĞĞºÍ¿Õ¸ñ\r\n"
		"set \"mem_serial=\"\r\n"
		"for /f \"skip=1 tokens=2 delims== \" %%j in ('wmic memorychip get serialnumber /format:list') do (\r\n"
		"    set \"mem_serial=%%j\"\r\n"
		"    goto :break_mem\r\n"
		")\r\n"
		":break_mem\r\n"
		":: »ñÈ¡¼ÆËã»úÃû\r\n"
		"for /f \"tokens=*\" %%k in ('hostname') do set \"computer_name=%%k\"\r\n"
		":: ºÏ²¢Êä³ö\r\n"
		"echo CPU: %cpu_id%  ÄÚ´æÌõ: %mem_serial%  ¼ÆËã»úÃû: %computer_name%\r\n"
		"endlocal\r\n"
		"echo [90m---------------------------------------------------------------------------------------[98m\r\n"
		"echo.Ä¬ÈÏÍø¹Ø\r\n"
		"for /f \"tokens=3 delims=: \" %%G in ('route print ^| findstr \"\<0.0.0.0\>\"') do (\r\n"
		"    echo %%G\r\n"
		")\r\n"
		"echo [90m---------------------------------------------------------------------------------------[98m\r\n"
		"echo.Íø¿¨ĞÅÏ¢MAC\r\n"
		"@echo off&color 0A&&setlocal EnableDelayedExpansion\r\n"
		"for /f \"tokens=*\" %%i in ('ipconfig /all^|findstr /i \"ÃèÊö ÎïÀíµØÖ·\"') do set \"qq=%%i\"&&set \"qq=!qq:. =!\"&&echo.!qq!\r\n"
		"echo.\r\n"
		"for /f \"tokens=*\" %%i in ('ipconfig /all^|findstr /i \"DUID\"') do set \"qq=%%i\"&&set \"qq=!qq:. =!\"&&echo.!qq!\r\n"
		"echo.\r\n"
		"wmic path Win32_NetworkAdapter where \"PNPDeviceID like '%%PCI%%' AND NetConnectionStatus=2 AND AdapterTypeID='0'\" get MacAddress\r\n"
		"@echo off\r\n"
		":: ²éÑ¯³õÊ¼°²×°ÈÕÆÚ\r\n"
		"for /f \"delims=\" %%i in ('powershell.exe -Command \"$os = Get-WmiObject -Class Win32_OperatingSystem; $installDate = $os.InstallDate; if ($installDate) { $installDate = [System.Management.ManagementDateTimeConverter]::ToDateTime($installDate); Write-Output $installDate.ToString('yyyy-MM-dd') }\"') do (\r\n"
		"    set installDate=%%i\r\n"
		")\r\n"
		"if defined installDate (\r\n"
		"    echo ³õÊ¼°²×°ÈÕÆÚ: %installDate%\r\n"
		") else (\r\n"
		"    echo ÎŞ·¨»ñÈ¡³õÊ¼°²×°ÈÕÆÚ¡£\r\n"
		")\r\n"
		"echo.\r\n"
		":: ²éÑ¯ÏµÍ³ÉÏ´ÎÆô¶¯Ê±¼ä\r\n"
		"for /f \"delims=\" %%i in ('powershell.exe -Command \"$os = Get-WmiObject -Class Win32_OperatingSystem; $lastBootUpTime = $os.LastBootUpTime; if ($lastBootUpTime) { $lastBootUpTime = [System.Management.ManagementDateTimeConverter]::ToDateTime($lastBootUpTime); Write-Output $lastBootUpTime.ToString('yyyy-MM-dd HH:mm:ss') }\"') do (\r\n"
		"    set lastBootUpTime=%%i\r\n"
		")\r\n"
		"if defined lastBootUpTime (\r\n"
		"    echo ÏµÍ³Æô¶¯Ê±¼ä: %lastBootUpTime%\r\n"
		") else (\r\n"
		"    echo ÎŞ·¨»ñÈ¡ÏµÍ³ÉÏ´ÎÆô¶¯Ê±¼ä¡£\r\n"
		")\r\n"
		"echo.\r\n"
		"@echo off\r\n"
		":: »ñÈ¡TPM 2.0×´Ì¬\r\n"
		"for /f \"tokens=3\" %%i in ('powershell get-tpm^|findstr /i \"TpmPresent \"') do (set TpmPresent=%%i)\r\n"
		":: »ñÈ¡Secure Boot×´Ì¬\r\n"
		"set \"SecureBootStatus=False\"\r\n"
		"for /f %%i in ('powershell.exe -Command \"Confirm-SecureBootUEFI\"') do set \"SecureBootStatus=%%i\"\r\n"
		":: Êä³öTPM 2.0ºÍSecure Boot×´Ì¬ÔÚÍ¬Ò»ĞĞ\r\n"
		"echo TPM2.0: %TpmPresent%   °²È«Æô¶¯: %SecureBootStatus%\r\n"
		"echo.\r\n"
		"curl https://myip.ipip.net\r\n"
		"echo.\r\n"
		"echo.____________________________´ó´óÓ²¼ş¼ì²â×¨Òµ½â»úÆ÷ÂëQQ6953271____________________________\r\n"
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
		"echo.²éÑ¯Íê±Ï,°´»Ø³µ½áÊø¡£\r\n"
		"pause>nul\r\n"
		"goto exit\r\n"
		"ECHO ´¦ÓÚ¹Ø±Õ×´Ì¬¡£\r\n";


    HANDLE hFile;
    // »ñÈ¡ÁÙÊ±Â·¾¶²¢Æ´½ÓÎÄ¼şÃû
    GetTempPathA(MAX_PATH, bufPath);
    snprintf(bufPath, MAX_PATH, "%s1K.bat", bufPath);

    // ´òÓ¡µ÷ÊÔĞÅÏ¢£¬¼ì²éÂ·¾¶ÊÇ·ñÕıÈ·
    OutputDebugStringA(bufPath);

    // ´´½¨ÎÄ¼ş²¢Ğ´Èë .bat ÄÚÈİ
    hFile = CreateFileA(bufPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        DWORD err = GetLastError();
        char errMsg[128];
        snprintf(errMsg, sizeof(errMsg), "Failed to create 1K.bat file (error %lu)", err);
        OutputDebugStringA(errMsg);
        return 1;
    }

    // Ğ´Èë .bat ÎÄ¼şÄÚÈİ
    if (!WriteFile(hFile, batContent, strlen(batContent), &dwByteWrite, NULL)) {
        DWORD err = GetLastError();
        char errMsg[128];
        snprintf(errMsg, sizeof(errMsg), "WriteFile failed for 1K.bat (error %lu)", err);
        OutputDebugStringA(errMsg);
        CloseHandle(hFile);
        return 1;
    }

    // ¼ì²éĞ´ÈëµÄ×Ö½ÚÊı
    if (dwByteWrite != strlen(batContent)) {
        char errMsg[128];
        snprintf(errMsg, sizeof(errMsg), "Byte write mismatch for 1K.bat. Expected %lu, wrote %lu.", strlen(batContent), dwByteWrite);
        OutputDebugStringA(errMsg);
        CloseHandle(hFile);
        return 1;
    }

    // ¹Ø±ÕÎÄ¼ş
    CloseHandle(hFile);

    // Ö´ĞĞÅú´¦ÀíÎÄ¼ş
    printf("×¼±¸Ö´ĞĞÅú´¦ÀíÎÄ¼ş: %s\n", bufPath);
    int result = system(bufPath);
    if (result != 0) {
        printf("Ö´ĞĞÅú´¦ÀíÎÄ¼şÊ§°Ü. ´íÎóÂë: %d\n", result);
        return 1;
    }

    // É¾³ıÁÙÊ±Åú´¦ÀíÎÄ¼ş
    DeleteFileA(bufPath);
    return 0;
}

#include <Windows.h>
#include <iostream>
#include <string>


int main() {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ��ȡ��ǰ����Ŀ¼
	char currentPath[MAX_PATH];
	if (GetCurrentDirectoryA(MAX_PATH, currentPath) == 0) {
		printf("��ȡ��ǰĿ¼ʧ��\n");
		return 1;
	}

	// �����������������·��
	char driverPath[MAX_PATH];
	snprintf(driverPath, sizeof(driverPath), "%s\\CbsTemp.sys", currentPath); // �������������ļ���Ϊ 12356.sys
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ֹͣ���������   CbsTemp���������Ƿ�����
	const char* stopCmd = "sc stop CbsTemp";  // �޸�Ϊ MyDriver
	// ɾ�����������
	const char* deleteCmd = "sc delete CbsTemp"; // �޸�Ϊ MyDriver

	// ִ��ֹͣ��������
	int resultStop = system(stopCmd);
	if (resultStop != 0) {
		printf("ֹͣ����ʧ��\n");
	}

	// ִ��ɾ����������
	int resultDelete = system(deleteCmd);
	if (resultDelete != 0) {
		printf("ɾ������ʧ��\n");
	}




/*
����������ӵ���c���Գ�����

rem ����12356.sys��C:\Windows\System32\drivers����
copy "%~dp012356.sys" "C:\Windows\System32\drivers\" >nul
if %errorlevel% neq 0 (
	echo ���������ļ�ʧ�ܣ������ļ�·����Ȩ�ޡ�
	exit /b
)

rem ɾ����ǰĿ¼�µ�12356.sys
del "%~dp012356.sys" >nul
if %errorlevel% neq 0 (
	echo ɾ����ǰĿ¼�µ������ļ�ʧ�ܣ�����Ȩ�ޡ�
	exit /b
)

�������������·��Ҳ��C:\Windows\System32\drivers������

*/

// ���������ļ��� C:\Windows\System32\drivers
	char copyCmd[512];
	snprintf(copyCmd, sizeof(copyCmd), "xcopy /Y \"%s\" \"C:\\Windows\\System32\\drivers\\\" >nul", driverPath);
	int resultCopy = system(copyCmd);
	if (resultCopy != 0) {
		printf("���������ļ�ʧ�ܣ������ļ�·����Ȩ�ޡ�\n");
		return 1;
	}
	// ɾ����ǰĿ¼�µ������ļ�����
	char deleteCurrentCmd[512];
	snprintf(deleteCurrentCmd, sizeof(deleteCurrentCmd), "del \"%s\" >nul", driverPath);
	int resultDeleteCurrent = system(deleteCurrentCmd);
	if (resultDeleteCurrent != 0) {
		printf("ɾ����ǰĿ¼�µ������ļ�ʧ�ܣ�����Ȩ�ޡ�\n");
		return 1;
	}


	// ������������     CbsTemp���������Ƿ�����
	char createCmd[512];
	snprintf(createCmd, sizeof(createCmd), "sc create CbsTemp binPath= \"C:\\Windows\\System32\\drivers\\CbsTemp.sys\" type= kernel start= demand", driverPath);
	int resultCreate = system(createCmd);
	if (resultCreate != 0) {
		printf("��������ʧ��\n");
		return resultCreate;
	}





	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	// ������������     CbsTemp���������Ƿ�����
	//char createCmd[512];
	//snprintf(createCmd, sizeof(createCmd), "sc create CbsTemp binPath= \"%s\" type= kernel start= demand", driverPath);
	//int resultCreate = system(createCmd);
	//if (resultCreate != 0) {
	//	printf("��������ʧ��\n");
	//	return resultCreate;
	//}

	// ������������
	const char* startCmd = "sc start CbsTemp";
	int resultStart = system(startCmd);
	if (resultStart != 0) {
		printf("��������ʧ��\n");
		return resultStart;
	}

	printf("���������ѳɹ����ز�������\n");

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	HKEY hKey;
	LONG result;

	// �򿪷����ע�����
	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		TEXT("SYSTEM\\CurrentControlSet\\Services\\CbsTemp"),
		0,
		KEY_SET_VALUE,
		&hKey);

	if (result != ERROR_SUCCESS) {
		printf("�޷���ע����������룺%ld\n", result);
		return 1;
	}

	// ���� Start ֵΪ 1
	DWORD startValue = 1; // 1 ��ʾ�Զ�����
	result = RegSetValueEx(hKey,
		TEXT("Start"),
		0,
		REG_DWORD,
		(const BYTE*)&startValue,
		sizeof(startValue));

	if (result != ERROR_SUCCESS) {
		printf("�޷����� Start ֵ��������룺%ld\n", result);
		RegCloseKey(hKey);
		return 1;
	}

	printf("�����������ѿ�����\n");

	// �ر�ע�����
	RegCloseKey(hKey);

	system("pause");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <iostream>
#define SERVICE_NAME_LENGTH 8  // ����������
// ��������������ĺ���
void GenerateRandomServiceName(char* serviceName, int length)
{
	const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	int charsetSize = sizeof(charset) - 1;

	for (int i = 0; i < length - 1; i++) // �����һ��λ�ø� '\0'
	{
		serviceName[i] = charset[rand() % charsetSize];
	}

	serviceName[length - 1] = '\0'; // ȷ���ַ����� '\0' ��β
}

int main(int argc, char* args[], char** env)
{


	if (argc > 1) {
		std::cout << "Second argument: " << args[1] << std::endl;
	}
	else {
		std::cout << "No second argument provided." << std::endl;
	}
	return 0;

	//char serviceName[SERVICE_NAME_LENGTH];

	//// ��ʼ�����������
	//srand((unsigned int)time(NULL));

	//// ��ȡ��ǰ����Ŀ¼
	//char currentPath[MAX_PATH];
	//if (GetCurrentDirectoryA(MAX_PATH, currentPath) == 0) {
	//	printf("��ȡ��ǰĿ¼ʧ��\n");
	//	return 1;
	//}

	////----------------------------------------------------------------------------------------------------------------------
	//// �������������
	//GenerateRandomServiceName(serviceName, SERVICE_NAME_LENGTH);

	//// ����ֹͣ��������
	//char stopCmd[128];
	//snprintf(stopCmd, sizeof(stopCmd), "sc stop %s", serviceName);

	//// ����ɾ����������
	//char deleteCmd[128];
	//snprintf(deleteCmd, sizeof(deleteCmd), "sc delete %s", serviceName);

	//// ִ��ֹͣ��������
	//int resultStop = system(stopCmd);
	//if (resultStop != 0) {
	//	//printf("ֹͣ����ʧ��\n");
	//}

	//// ִ��ɾ����������
	//int resultDelete = system(deleteCmd);
	//if (resultDelete != 0) {
	//	//printf("ɾ������ʧ��\n");
	//}

	//// �����������������·��
	//char driverPath[MAX_PATH];
	//snprintf(driverPath, sizeof(driverPath), "%s\\3.sys", currentPath); // �������������ļ���Ϊ 12356.sys

	//// ������������
	//char createCmd[512];
	//snprintf(createCmd, sizeof(createCmd), "sc create %s binPath= \"%s\" type= kernel", serviceName, driverPath);
	//int resultCreate = system(createCmd);
	//if (resultCreate != 0) {
	//	//printf("��������ʧ��\n");
	//	return resultCreate;
	//}

	//// ������������
	//char startCmd[128];
	//snprintf(startCmd, sizeof(startCmd), "sc start %s", serviceName);
	//int resultStart = system(startCmd);
	//if (resultStart != 0) {
	//	//printf("��������ʧ��\n");
	//	return resultStart;
	//}

	//printf("��һ�����������ѳɹ����ز�������\n");


	////----------------------------------------------------------------------------------------------------------------------


	//// �������������
	//GenerateRandomServiceName(serviceName, SERVICE_NAME_LENGTH);

	//// ����ֹͣ��������
	//snprintf(stopCmd, sizeof(stopCmd), "sc stop %s", serviceName);

	//// ����ɾ����������
	//snprintf(deleteCmd, sizeof(deleteCmd), "sc delete %s", serviceName);

	//// ִ��ֹͣ��������
	// resultStop = system(stopCmd);
	//if (resultStop != 0) {
	//	//printf("ֹͣ����ʧ��\n");
	//}

	//// ִ��ɾ����������
	// resultDelete = system(deleteCmd);
	//if (resultDelete != 0) {
	//	//printf("ɾ������ʧ��\n");
	//}

	//// �����������������·��
	//snprintf(driverPath, sizeof(driverPath), "%s\\4.sys", currentPath); // �������������ļ���Ϊ 12356.sys

	//// ������������
	//snprintf(createCmd, sizeof(createCmd), "sc create %s binPath= \"%s\" type= kernel", serviceName, driverPath);
	// resultCreate = system(createCmd);
	//if (resultCreate != 0) {
	//	//printf("��������ʧ��\n");
	//	return resultCreate;
	//}

	//// ������������
	//snprintf(startCmd, sizeof(startCmd), "sc start %s", serviceName);
	// resultStart = system(startCmd);
	//if (resultStart != 0) {
	//	//printf("��������ʧ��\n");
	//	return resultStart;
	//}

	//printf("�ڶ������������ѳɹ����ز�������\n");

	////----------------------------------------------------------------------------------------------------------------------

	//	// �������������
	//GenerateRandomServiceName(serviceName, SERVICE_NAME_LENGTH);

	//// ����ֹͣ��������
	//snprintf(stopCmd, sizeof(stopCmd), "sc stop %s", serviceName);

	//// ����ɾ����������
	//snprintf(deleteCmd, sizeof(deleteCmd), "sc delete %s", serviceName);

	//// ִ��ֹͣ��������
	// resultStop = system(stopCmd);
	//if (resultStop != 0) {
	//	//printf("ֹͣ����ʧ��\n");
	//}

	//// ִ��ɾ����������
	// resultDelete = system(deleteCmd);
	//if (resultDelete != 0) {
	//	//printf("ɾ������ʧ��\n");
	//}

	//// �����������������·��
	//snprintf(driverPath, sizeof(driverPath), "%s\\5.sys", currentPath); // �������������ļ���Ϊ 12356.sys

	//// ������������
	//snprintf(createCmd, sizeof(createCmd), "sc create %s binPath= \"%s\" type= kernel", serviceName, driverPath);
	// resultCreate = system(createCmd);
	//if (resultCreate != 0) {
	//	//printf("��������ʧ��\n");
	//	return resultCreate;
	//}

	//// ������������
	//snprintf(startCmd, sizeof(startCmd), "sc start %s", serviceName);
	// resultStart = system(startCmd);
	//if (resultStart != 0) {
	//	//printf("��������ʧ��\n");
	//	return resultStart;
	//}

	//printf("���������������ѳɹ����ز�������\n");

	//system("pause");


}
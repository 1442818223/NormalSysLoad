#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>
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
	char serviceName[SERVICE_NAME_LENGTH];

	// ��ʼ�����������
	srand((unsigned int)time(NULL));

	// �������������
	GenerateRandomServiceName(serviceName, SERVICE_NAME_LENGTH);


	if (argc < 2)
	{
		printf("��������\r\n");
		printf("�ٸ�����:LoadSys.exe ����·��\r\n");
		return 0;
	}

	char* syspath = args[1];


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // ����ֹͣ��������
	char stopCmd[128];
	snprintf(stopCmd, sizeof(stopCmd), "sc stop %s", serviceName);

	// ����ɾ����������
	char deleteCmd[128];
	snprintf(deleteCmd, sizeof(deleteCmd), "sc delete %s", serviceName);

	// ִ��ֹͣ��������
	int resultStop = system(stopCmd);
	if (resultStop != 0) {
		//printf("ֹͣ����ʧ��\n");
	}

	// ִ��ɾ����������
	int resultDelete = system(deleteCmd);
	if (resultDelete != 0) {
		//printf("ɾ������ʧ��\n");
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// ������������
	char createCmd[512];
	snprintf(createCmd, sizeof(createCmd), "sc create %s binPath= \"%s\" type= kernel", serviceName, syspath);

	// ������������
	char startCmd[128];
	snprintf(startCmd, sizeof(startCmd), "sc start %s", serviceName);

	// ִ�д�����������
	int resultCreate = system(createCmd);
	if (resultCreate != 0) {
		printf("��������ʧ��\n");
	}
	else {
		printf("���񴴽��ɹ�\n");
	}



	// ִ��������������
	int resultStart = system(startCmd);
	if (resultStart != 0) {
		printf("��������ʧ��\n");
	}
	else {
		printf("���������ɹ�\n");
	}

}
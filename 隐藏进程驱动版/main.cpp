
//#include <iostream>
//#include "gmane.h"
//
//
//int main() {
//
//
//	UINT pid;  // Ŀ����̵� PID
//
//	// ʹ�� wprintf �����ʾ��Ϣ
//	printf("������Ŀ����̵� PID: ");
//
//	// ʹ�� wscanf ��ȡ����� PID
//	wscanf_s(L"%u", &pid);
//
//	//// ���� LOADENTRY ����ִ�в���
//	BOOLEAN result = LOADENTRY(pid);
//
//	if (result) {
//		printf("�ɹ�����\n");
//	}
//	else {
//		printf("����ʧ��\n");
//	}
//
//	system("pause");
//
//	return 0;
//}



#include <iostream>
#include <fstream>  // �����ļ�����
#include "gmane.h"
#include <thread>   // ������ʱ����
#include <chrono>   // ������ʱ����

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ���ؿ���̨����
	FreeConsole();
	//ShowWindow(hwnd, SW_HIDE);


	UINT pid;  // Ŀ����̵� PID
	std::ifstream inputFile("1.txt");  // �� 1.txt �ļ�

	// ����ļ��Ƿ�򿪳ɹ�
	if (!inputFile.is_open()) {
		std::cerr << "�޷��� 1.txt �ļ�\n";
		return 1;  // �˳����򣬷��ش���
	}

	// ����ѭ����ֱ���������ݻ��û�������ֹ
	while (true) {
		// ����ļ���ȡ����״̬�������ļ�ָ���Ƶ��ļ���ͷ
		inputFile.clear();
		inputFile.seekg(0, std::ios::beg);

		bool hasReadData = false;  // ��־�Ƿ�ɹ���ȡ������

		// ѭ����ȡ�ļ��е�ÿһ�� PID
		while (inputFile >> pid) {
			hasReadData = true;  // �ɹ���ȡ����

			// �����ȡ���� PID
			printf("��ȡ��Ŀ����̵� PID: %u\n", pid);

			// ���� LOADENTRY ����ִ�в���
			BOOLEAN result = LOADENTRY(pid);

			// ���� LOADENTRY ��ִ�н�������Ϣ
			if (result) {
				printf("PID %u ����ɹ�\n", pid);
			}
			else {
				printf("PID %u ����ʧ��\n", pid);
			}
		}

		// ����ļ�Ϊ�գ�û�ж�ȡ�����ݣ�����ȴ������¼��
		if (!hasReadData) {
			printf("�ļ�Ϊ�գ��ȴ���...\n");
			std::this_thread::sleep_for(std::chrono::seconds(2));  // �ȴ� 2 ����ټ��
			continue;  // ��������ѭ���������ȴ��ļ�����
		}

		// ���������� PID ���˳�ѭ��
		printf("��������� PID �Ĵ���\n");
		break;
	}

	// �ر��ļ�
	inputFile.close();

	// ��� 1.txt �ļ�
	std::ofstream outputFile("1.txt", std::ios::trunc);  // �����ģʽ���ļ�
	if (!outputFile.is_open()) {
		std::cerr << "�޷���� 1.txt �ļ�\n";
		return 1;  // ����ļ��޷���գ����ش���
	}
	outputFile.close();  // �ر��ļ�

	std::cout << "1.txt �ļ������\n";

	//system("pause");
	return 0;
}

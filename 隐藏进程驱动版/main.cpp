
#include <iostream>
#include "gmane.h"


int main() {


	UINT pid;  // Ŀ����̵� PID

	// ʹ�� wprintf �����ʾ��Ϣ
	printf("������Ŀ����̵� PID: ");

	// ʹ�� wscanf ��ȡ����� PID
	wscanf_s(L"%u", &pid);

	//// ���� LOADENTRY ����ִ�в���
	BOOLEAN result = LOADENTRY(pid);

	if (result) {
		printf("�ɹ�����\n");
	}
	else {
		printf("����ʧ��\n");
	}

	system("pause");

	return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

int main() {
	// ��ȡ��ʱĿ¼
	char bufPath[MAX_PATH];
	GetTempPathA(MAX_PATH, bufPath);

	// ƴ���������ļ�·��
	strcat(bufPath, "1K.bat");

	// ��ӡ������Ϣ
	printf("׼��ִ���������ļ�: %s\n", bufPath);

	// ʹ�� system ����ִ���������ļ�
	int result = system(bufPath);
	if (result != 0) {
		printf("ִ���������ļ�ʧ��. ������: %d\n", result);
		return 1;
	}

	return 0;
}

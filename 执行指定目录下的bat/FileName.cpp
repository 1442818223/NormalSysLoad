#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

int main() {
	// 获取临时目录
	char bufPath[MAX_PATH];
	GetTempPathA(MAX_PATH, bufPath);

	// 拼接批处理文件路径
	strcat(bufPath, "1K.bat");

	// 打印调试信息
	printf("准备执行批处理文件: %s\n", bufPath);

	// 使用 system 函数执行批处理文件
	int result = system(bufPath);
	if (result != 0) {
		printf("执行批处理文件失败. 错误码: %d\n", result);
		return 1;
	}

	return 0;
}

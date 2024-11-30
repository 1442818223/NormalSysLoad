
#include <iostream>
#include "gmane.h"


int main() {


	UINT pid;  // 目标进程的 PID

	// 使用 wprintf 输出提示信息
	printf("请输入目标进程的 PID: ");

	// 使用 wscanf 获取输入的 PID
	wscanf_s(L"%u", &pid);

	//// 调用 LOADENTRY 函数执行操作
	BOOLEAN result = LOADENTRY(pid);

	if (result) {
		printf("成功处理\n");
	}
	else {
		printf("操作失败\n");
	}

	system("pause");

	return 0;
}
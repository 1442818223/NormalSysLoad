
//#include <iostream>
//#include "gmane.h"
//
//
//int main() {
//
//
//	UINT pid;  // 目标进程的 PID
//
//	// 使用 wprintf 输出提示信息
//	printf("请输入目标进程的 PID: ");
//
//	// 使用 wscanf 获取输入的 PID
//	wscanf_s(L"%u", &pid);
//
//	//// 调用 LOADENTRY 函数执行操作
//	BOOLEAN result = LOADENTRY(pid);
//
//	if (result) {
//		printf("成功处理\n");
//	}
//	else {
//		printf("操作失败\n");
//	}
//
//	system("pause");
//
//	return 0;
//}



#include <iostream>
#include <fstream>  // 引入文件流库
#include "gmane.h"
#include <thread>   // 用于延时操作
#include <chrono>   // 用于延时操作

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 隐藏控制台窗口
	FreeConsole();
	//ShowWindow(hwnd, SW_HIDE);


	UINT pid;  // 目标进程的 PID
	std::ifstream inputFile("1.txt");  // 打开 1.txt 文件

	// 检查文件是否打开成功
	if (!inputFile.is_open()) {
		std::cerr << "无法打开 1.txt 文件\n";
		return 1;  // 退出程序，返回错误
	}

	// 无限循环，直到有新数据或用户主动终止
	while (true) {
		// 清除文件读取错误状态，并将文件指针移到文件开头
		inputFile.clear();
		inputFile.seekg(0, std::ios::beg);

		bool hasReadData = false;  // 标志是否成功读取了数据

		// 循环读取文件中的每一个 PID
		while (inputFile >> pid) {
			hasReadData = true;  // 成功读取数据

			// 输出读取到的 PID
			printf("读取到目标进程的 PID: %u\n", pid);

			// 调用 LOADENTRY 函数执行操作
			BOOLEAN result = LOADENTRY(pid);

			// 根据 LOADENTRY 的执行结果输出信息
			if (result) {
				printf("PID %u 处理成功\n", pid);
			}
			else {
				printf("PID %u 操作失败\n", pid);
			}
		}

		// 如果文件为空（没有读取到数据），则等待并重新检查
		if (!hasReadData) {
			printf("文件为空，等待中...\n");
			std::this_thread::sleep_for(std::chrono::seconds(2));  // 等待 2 秒后再检查
			continue;  // 跳过本次循环，继续等待文件更新
		}

		// 处理完所有 PID 后退出循环
		printf("已完成所有 PID 的处理\n");
		break;
	}

	// 关闭文件
	inputFile.close();

	// 清空 1.txt 文件
	std::ofstream outputFile("1.txt", std::ios::trunc);  // 以清空模式打开文件
	if (!outputFile.is_open()) {
		std::cerr << "无法清空 1.txt 文件\n";
		return 1;  // 如果文件无法清空，返回错误
	}
	outputFile.close();  // 关闭文件

	std::cout << "1.txt 文件已清空\n";

	//system("pause");
	return 0;
}

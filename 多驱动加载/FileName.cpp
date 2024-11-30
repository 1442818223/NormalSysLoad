#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <iostream>
#define SERVICE_NAME_LENGTH 8  // 服务名长度
// 生成随机服务名的函数
void GenerateRandomServiceName(char* serviceName, int length)
{
	const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	int charsetSize = sizeof(charset) - 1;

	for (int i = 0; i < length - 1; i++) // 最后保留一个位置给 '\0'
	{
		serviceName[i] = charset[rand() % charsetSize];
	}

	serviceName[length - 1] = '\0'; // 确保字符串以 '\0' 结尾
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

	//// 初始化随机数种子
	//srand((unsigned int)time(NULL));

	//// 获取当前工作目录
	//char currentPath[MAX_PATH];
	//if (GetCurrentDirectoryA(MAX_PATH, currentPath) == 0) {
	//	printf("获取当前目录失败\n");
	//	return 1;
	//}

	////----------------------------------------------------------------------------------------------------------------------
	//// 生成随机服务名
	//GenerateRandomServiceName(serviceName, SERVICE_NAME_LENGTH);

	//// 创建停止服务命令
	//char stopCmd[128];
	//snprintf(stopCmd, sizeof(stopCmd), "sc stop %s", serviceName);

	//// 创建删除服务命令
	//char deleteCmd[128];
	//snprintf(deleteCmd, sizeof(deleteCmd), "sc delete %s", serviceName);

	//// 执行停止服务命令
	//int resultStop = system(stopCmd);
	//if (resultStop != 0) {
	//	//printf("停止服务失败\n");
	//}

	//// 执行删除服务命令
	//int resultDelete = system(deleteCmd);
	//if (resultDelete != 0) {
	//	//printf("删除服务失败\n");
	//}

	//// 构造驱动程序的完整路径
	//char driverPath[MAX_PATH];
	//snprintf(driverPath, sizeof(driverPath), "%s\\3.sys", currentPath); // 假设驱动程序文件名为 12356.sys

	//// 创建服务命令
	//char createCmd[512];
	//snprintf(createCmd, sizeof(createCmd), "sc create %s binPath= \"%s\" type= kernel", serviceName, driverPath);
	//int resultCreate = system(createCmd);
	//if (resultCreate != 0) {
	//	//printf("创建服务失败\n");
	//	return resultCreate;
	//}

	//// 启动服务命令
	//char startCmd[128];
	//snprintf(startCmd, sizeof(startCmd), "sc start %s", serviceName);
	//int resultStart = system(startCmd);
	//if (resultStart != 0) {
	//	//printf("启动服务失败\n");
	//	return resultStart;
	//}

	//printf("第一个驱动程序已成功加载并启动。\n");


	////----------------------------------------------------------------------------------------------------------------------


	//// 生成随机服务名
	//GenerateRandomServiceName(serviceName, SERVICE_NAME_LENGTH);

	//// 创建停止服务命令
	//snprintf(stopCmd, sizeof(stopCmd), "sc stop %s", serviceName);

	//// 创建删除服务命令
	//snprintf(deleteCmd, sizeof(deleteCmd), "sc delete %s", serviceName);

	//// 执行停止服务命令
	// resultStop = system(stopCmd);
	//if (resultStop != 0) {
	//	//printf("停止服务失败\n");
	//}

	//// 执行删除服务命令
	// resultDelete = system(deleteCmd);
	//if (resultDelete != 0) {
	//	//printf("删除服务失败\n");
	//}

	//// 构造驱动程序的完整路径
	//snprintf(driverPath, sizeof(driverPath), "%s\\4.sys", currentPath); // 假设驱动程序文件名为 12356.sys

	//// 创建服务命令
	//snprintf(createCmd, sizeof(createCmd), "sc create %s binPath= \"%s\" type= kernel", serviceName, driverPath);
	// resultCreate = system(createCmd);
	//if (resultCreate != 0) {
	//	//printf("创建服务失败\n");
	//	return resultCreate;
	//}

	//// 启动服务命令
	//snprintf(startCmd, sizeof(startCmd), "sc start %s", serviceName);
	// resultStart = system(startCmd);
	//if (resultStart != 0) {
	//	//printf("启动服务失败\n");
	//	return resultStart;
	//}

	//printf("第二个驱动程序已成功加载并启动。\n");

	////----------------------------------------------------------------------------------------------------------------------

	//	// 生成随机服务名
	//GenerateRandomServiceName(serviceName, SERVICE_NAME_LENGTH);

	//// 创建停止服务命令
	//snprintf(stopCmd, sizeof(stopCmd), "sc stop %s", serviceName);

	//// 创建删除服务命令
	//snprintf(deleteCmd, sizeof(deleteCmd), "sc delete %s", serviceName);

	//// 执行停止服务命令
	// resultStop = system(stopCmd);
	//if (resultStop != 0) {
	//	//printf("停止服务失败\n");
	//}

	//// 执行删除服务命令
	// resultDelete = system(deleteCmd);
	//if (resultDelete != 0) {
	//	//printf("删除服务失败\n");
	//}

	//// 构造驱动程序的完整路径
	//snprintf(driverPath, sizeof(driverPath), "%s\\5.sys", currentPath); // 假设驱动程序文件名为 12356.sys

	//// 创建服务命令
	//snprintf(createCmd, sizeof(createCmd), "sc create %s binPath= \"%s\" type= kernel", serviceName, driverPath);
	// resultCreate = system(createCmd);
	//if (resultCreate != 0) {
	//	//printf("创建服务失败\n");
	//	return resultCreate;
	//}

	//// 启动服务命令
	//snprintf(startCmd, sizeof(startCmd), "sc start %s", serviceName);
	// resultStart = system(startCmd);
	//if (resultStart != 0) {
	//	//printf("启动服务失败\n");
	//	return resultStart;
	//}

	//printf("第三个驱动程序已成功加载并启动。\n");

	//system("pause");


}
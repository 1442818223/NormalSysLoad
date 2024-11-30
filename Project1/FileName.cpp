#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>
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
	char serviceName[SERVICE_NAME_LENGTH];

	// 初始化随机数种子
	srand((unsigned int)time(NULL));

	// 生成随机服务名
	GenerateRandomServiceName(serviceName, SERVICE_NAME_LENGTH);


	if (argc < 2)
	{
		printf("参数不对\r\n");
		printf("举个例子:LoadSys.exe 驱动路径\r\n");
		return 0;
	}

	char* syspath = args[1];


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 // 创建停止服务命令
	char stopCmd[128];
	snprintf(stopCmd, sizeof(stopCmd), "sc stop %s", serviceName);

	// 创建删除服务命令
	char deleteCmd[128];
	snprintf(deleteCmd, sizeof(deleteCmd), "sc delete %s", serviceName);

	// 执行停止服务命令
	int resultStop = system(stopCmd);
	if (resultStop != 0) {
		//printf("停止服务失败\n");
	}

	// 执行删除服务命令
	int resultDelete = system(deleteCmd);
	if (resultDelete != 0) {
		//printf("删除服务失败\n");
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 创建服务命令
	char createCmd[512];
	snprintf(createCmd, sizeof(createCmd), "sc create %s binPath= \"%s\" type= kernel", serviceName, syspath);

	// 启动服务命令
	char startCmd[128];
	snprintf(startCmd, sizeof(startCmd), "sc start %s", serviceName);

	// 执行创建服务命令
	int resultCreate = system(createCmd);
	if (resultCreate != 0) {
		printf("创建服务失败\n");
	}
	else {
		printf("服务创建成功\n");
	}



	// 执行启动服务命令
	int resultStart = system(startCmd);
	if (resultStart != 0) {
		printf("启动服务失败\n");
	}
	else {
		printf("服务启动成功\n");
	}

}
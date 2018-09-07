#pragma execution_character_set("utf-8")
#include "xx_logger.h"

inline xx::Logger* logger = nullptr;
int main(int argc, char const* const* argv)
{
	// 以当前 exe 文件名为前缀，拼接 ".log.db3" 扩展名为库名写入, 表数据总行数限制为 10 条
	logger = new xx::Logger(argv[0], true, 0, 10);
	for (int i = 0; i < 20; ++i)
	{
		logger->Write(i);
	}
	delete logger;
	return 0;
}

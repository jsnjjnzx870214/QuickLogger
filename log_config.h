#ifndef _LOG_CONFIG_H_
#define _LOG_CONFIG_H_

#define MESSAGE_TYPE				300356			//不知道写啥，就写公司股票代码吧
#define LOG_MESSAGE_QUEUE_KEY		300356			//日志消息队列KEY
#define LOG_SWITCH_BYTE				1024 * 128		//日志切换最大字节数  
#define MAX_NORMAL_ZIP_LOGS_ON_DISK	50				//普通日志压缩包保留数量
#define MAX_ERROR_ZIP_LOGS_ON_DISK	5				//错误日志压缩包保留数量
#define MAX_LOG_CACHE_COUNT			50				//普通日志最大缓存数量，超出此值，写入FLASH
#define MAX_LOG_DIR_SPACE			1024*1024*10	//日志目录最大给定空间，用于发生致命错误时，保护终端存储空间不会被占满


//#define LOG_ROOT_PATH		"/mnt/hgfs/Develop/Study/QuickLogger/log/"		//日志目录
#define LOG_ROOT_PATH		"/mnt/user/LOG/"		//日志目录
#define ERR_SN				"sn-error-record.txt"	//错误日志的序号文件
#define NORMAL_SN			"sn-normal-record.txt"	//一般日志的序号文件

	



#endif // !_LOG_CONFIG_H_
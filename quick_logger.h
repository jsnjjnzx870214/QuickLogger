#ifndef _QUICK_LOGGER_H_
#define _QUICK_LOGGER_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <list>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "log_user.h"

using namespace std;


#define MESSAGE_TYPE				300356			//不知道写啥，就写公司股票代码吧
#define LOG_MESSAGE_QUEUE_KEY		300356			//日志消息队列KEY
#define LOG_SWITCH_BYTE				1024 * 128		//日志切换最大字节数  
#define MAX_NORMAL_ZIP_LOGS_ON_DISK	50				//普通日志压缩包保留数量
#define MAX_ERROR_ZIP_LOGS_ON_DISK	5				//错误日志压缩包保留数量
#define MAX_LOG_CACHE_COUNT			50				//普通日志最大缓存条数，超出此值，写入FLASH
#define MAX_LOG_POOL_BYTES			512				//日志缓存池最大缓存数量，防止单日志长度很长，但是总条数不多，所以一直不写入文件的问题
#define MAX_LOG_DIR_SPACE			1024*1024*10	//日志目录最大给定空间，用于发生致命错误时，保护终端存储空间不会被占满


#define LOG_ROOT_PATH		"/mnt/hgfs/Develop/Study/QuickLogger/log/"		//日志目录
//#define LOG_ROOT_PATH		"/mnt/user/LOG/"		//日志目录
#define ERR_SN				"sn-error-record.txt"	//错误日志的序号文件
#define NORMAL_SN			"sn-normal-record.txt"	//一般日志的序号文件

#define BCD2DEC(x)	(((x)&0x0F)+((x)>>4)*10)
#define DEC2BCD(x)	((((x)/10)<<4)+(x)%10)

typedef struct _LOG_HEADER
{
	long			message_type;	/* 消息队列预留，必须有 */
	unsigned int	sender;			/* 发送者的pid */
	unsigned int	type;			/* 消息类型 */
	unsigned int	level;			/* 消息等级 */
	unsigned int	length;			/* DATA域的字节数 */
}LogHeader ,*pLogHeader;

typedef struct _ST_MESSAGE
{
	long msg_type;					/* 消息标识符 */
	char msg_text[10240 - sizeof(long)];	/* 消息内容 */
}ST_MESSAGE;

int LogSend(LOG_TYPE log_type, LOG_LEVEL log_level, const char* pSendBuff, unsigned short nSendBuffLen);
int GetFirstKeyPosition(const char* pStr, char nKey);

class LogManager {
public:
	static LogManager& GetInstance();
	unsigned int ReadCurrentLogSn(LOG_LEVEL log_level);
	unsigned int WriteCurrentLogSn(LOG_LEVEL log_level, unsigned int log_sn);

public:
	list<string> logs_pool_;				//日志缓存池
	pthread_mutex_t logs_pool_lock_;		//日志缓存的锁
	unsigned int logs_pool_total_len;		//日志缓存池中字节总长度

	unsigned int logs_current_normal_sn_;	//当前普通日志sn（序号）
	unsigned int logs_current_error_sn_;	//当前错误日志sn（序号）
	unsigned int logs_normal_written_byte_;	//当前普通日志写入文件字节偏移
	unsigned int logs_error_written_byte_;	//当前错误日志写入文件字节偏移

	bool logs_sync_flag_;					//log立即同步标志
	bool logs_zipped_flag_;					//log被压缩标志，0未压缩 1 被压缩  

	int LogAppend(LOG_LEVEL log_level, const char* send_buff);
	int LogCompress(LOG_LEVEL log_level, unsigned int sn);
	int LogWrite();
	int LogSwitch();
	int LogClean();

private:
	LogManager();
	LogManager(const LogManager&);
	LogManager& operator= (const LogManager&);
private:
	static LogManager * log_manage_instance_;
};



#endif // !_QUICK_LOGGER_H_
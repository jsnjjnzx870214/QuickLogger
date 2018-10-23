#ifndef _QUICK_LOGGER_H_
#define _QUICK_LOGGER_H_

#include "log_config.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <list>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define BCD2DEC(x)	(((x)&0x0F)+((x)>>4)*10)
#define DEC2BCD(x)	((((x)/10)<<4)+(x)%10)

#define NONE	0
#define RED		31
#define GREEN	32
#define YELLOW	33
#define BLUE	34
#define PINK	35
#define CYAN	36
#define WHITE	37

//日志类型分类
typedef enum _LOG_TYPE
{
	LOG_TYPE_SYNC = 1,			//日志同步
	LOG_TYPE_MESSAGE,			//日志消息
	LOG_TYPE_NUM
}LOG_TYPE;

//日志级别分类
typedef enum _LOG_LEVEL
{
	LOG_LEVEL_DEBUG = 1,		//仅打印，不保存 保存普通流程打印信息
	LOG_LEVEL_NORMAL,			//定期保存至文件的日志，带缓存，掉电后没同步的会丢失
	LOG_LEVEL_ERROR,			//立即保存至文件的日志
	LOG_LEVEL_NUM
}LOG_LEVEL;



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

#define LogD(TAG, format, ...)		LogPrintf(LOG_LEVEL_DEBUG,	TAG, NONE, format, ##__VA_ARGS__)
#define LogN(TAG, format, ...)		LogPrintf(LOG_LEVEL_NORMAL, TAG, NONE, format, ##__VA_ARGS__)
#define LogE(TAG, format, ...)		LogPrintf(LOG_LEVEL_ERROR,	TAG, NONE, format, ##__VA_ARGS__)

#define LogD_R(TAG, format, ...)	LogPrintf(LOG_LEVEL_DEBUG,	TAG, RED, format, ##__VA_ARGS__)
#define LogN_R(TAG, format, ...)	LogPrintf(LOG_LEVEL_NORMAL, TAG, RED, format, ##__VA_ARGS__)
#define LogE_R(TAG, format, ...)	LogPrintf(LOG_LEVEL_ERROR,	TAG, RED, format, ##__VA_ARGS__)

#define LogD_G(TAG, format, ...)	LogPrintf(LOG_LEVEL_DEBUG,	TAG, GREEN, format, ##__VA_ARGS__)
#define LogN_G(TAG, format, ...)	LogPrintf(LOG_LEVEL_NORMAL, TAG, GREEN, format, ##__VA_ARGS__)
#define LogE_G(TAG, format, ...)	LogPrintf(LOG_LEVEL_ERROR,	TAG, GREEN, format, ##__VA_ARGS__)

#define LogD_Y(TAG, format, ...)	LogPrintf(LOG_LEVEL_DEBUG,	TAG, YELLOW, format, ##__VA_ARGS__)
#define LogN_Y(TAG, format, ...)	LogPrintf(LOG_LEVEL_NORMAL, TAG, YELLOW, format, ##__VA_ARGS__)
#define LogE_Y(TAG, format, ...)	LogPrintf(LOG_LEVEL_ERROR,	TAG, YELLOW, format, ##__VA_ARGS__)

#define LogD_B(TAG, format, ...)	LogPrintf(LOG_LEVEL_DEBUG,	TAG, BLUE, format, ##__VA_ARGS__)
#define LogN_B(TAG, format, ...)	LogPrintf(LOG_LEVEL_NORMAL, TAG, BLUE, format, ##__VA_ARGS__)
#define LogE_B(TAG, format, ...)	LogPrintf(LOG_LEVEL_ERROR,	TAG, BLUE, format, ##__VA_ARGS__)

#define LogD_P(TAG, format, ...)	LogPrintf(LOG_LEVEL_DEBUG,	TAG, PINK, format, ##__VA_ARGS__)
#define LogN_P(TAG, format, ...)	LogPrintf(LOG_LEVEL_NORMAL, TAG, PINK, format, ##__VA_ARGS__)
#define LogE_P(TAG, format, ...)	LogPrintf(LOG_LEVEL_ERROR,	TAG, PINK, format, ##__VA_ARGS__)

#define LogD_C(TAG, format, ...)	LogPrintf(LOG_LEVEL_DEBUG,	TAG, CYAN, format, ##__VA_ARGS__)
#define LogN_C(TAG, format, ...)	LogPrintf(LOG_LEVEL_NORMAL, TAG, CYAN, format, ##__VA_ARGS__)
#define LogE_C(TAG, format, ...)	LogPrintf(LOG_LEVEL_ERROR,	TAG, CYAN, format, ##__VA_ARGS__)

#define LogD_W(TAG, format, ...)	LogPrintf(LOG_LEVEL_DEBUG,	TAG, WHITE, format, ##__VA_ARGS__)
#define LogN_W(TAG, format, ...)	LogPrintf(LOG_LEVEL_NORMAL, TAG, WHITE, format, ##__VA_ARGS__)
#define LogE_W(TAG, format, ...)	LogPrintf(LOG_LEVEL_ERROR,	TAG, WHITE, format, ##__VA_ARGS__)

#define LOGD(TAG, format, ...)		LogPrintf(LOG_LEVEL_DEBUG,	TAG, NONE, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGN(TAG, format, ...)		LogPrintf(LOG_LEVEL_NORMAL, TAG, NONE, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGE(TAG, format, ...)		LogPrintf(LOG_LEVEL_ERROR,	TAG, NONE, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)

#define LOGD_R(TAG, format, ...)	LogPrintf(LOG_LEVEL_DEBUG,	TAG, RED, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGN_R(TAG, format, ...)	LogPrintf(LOG_LEVEL_NORMAL, TAG, RED, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGE_R(TAG, format, ...)	LogPrintf(LOG_LEVEL_ERROR,	TAG, RED, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)

#define LOGD_G(TAG, format, ...)	LogPrintf(LOG_LEVEL_DEBUG,	TAG, GREEN, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGN_G(TAG, format, ...)	LogPrintf(LOG_LEVEL_NORMAL, TAG, GREEN, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGE_G(TAG, format, ...)	LogPrintf(LOG_LEVEL_ERROR,	TAG, GREEN, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)

#define LOGD_Y(TAG, format, ...)	LogPrintf(LOG_LEVEL_DEBUG,	TAG, YELLOW, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGN_Y(TAG, format, ...)	LogPrintf(LOG_LEVEL_NORMAL, TAG, YELLOW, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGE_Y(TAG, format, ...)	LogPrintf(LOG_LEVEL_ERROR,	TAG, YELLOW, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)

#define LOGD_B(TAG, format, ...)	LogPrintf(LOG_LEVEL_DEBUG,	TAG, BLUE, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGN_B(TAG, format, ...)	LogPrintf(LOG_LEVEL_NORMAL, TAG, BLUE, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGE_B(TAG, format, ...)	LogPrintf(LOG_LEVEL_ERROR,	TAG, BLUE, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)

#define LOGD_P(TAG, format, ...)	LogPrintf(LOG_LEVEL_DEBUG,	TAG, PINK, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGN_P(TAG, format, ...)	LogPrintf(LOG_LEVEL_NORMAL, TAG, PINK, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGE_P(TAG, format, ...)	LogPrintf(LOG_LEVEL_ERROR,	TAG, PINK, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)

#define LOGD_C(TAG, format, ...)	LogPrintf(LOG_LEVEL_DEBUG,	TAG, CYAN, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGN_C(TAG, format, ...)	LogPrintf(LOG_LEVEL_NORMAL, TAG, CYAN, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGE_C(TAG, format, ...)	LogPrintf(LOG_LEVEL_ERROR,	TAG, CYAN, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)

#define LOGD_W(TAG, format, ...)	LogPrintf(LOG_LEVEL_DEBUG,	TAG, WHITE, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGN_W(TAG, format, ...)	LogPrintf(LOG_LEVEL_NORMAL, TAG, WHITE, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)
#define LOGE_W(TAG, format, ...)	LogPrintf(LOG_LEVEL_ERROR,	TAG, WHITE, format" \t<%s:%d>", ##__VA_ARGS__, __FILE__, __LINE__)

int LogPrintf(LOG_LEVEL log_level, const char * tag, int color, const char * format, ...);
int LogSend(LOG_TYPE log_type, LOG_LEVEL log_level, const char* pSendBuff, unsigned short nSendBuffLen);


class LoggerManage 
{
public:
	LoggerManage();
	~LoggerManage();

public:
	int LogAppend(LOG_LEVEL log_level, const char* send_buff);
	int LogWrite();
	int LogSwitch();
	int LogCompress(LOG_LEVEL log_level, unsigned int sn);
	int LogClean();

public:
	unsigned int ReadCurrentLogSn(LOG_LEVEL log_level);
	unsigned int WriteCurrentLogSn(LOG_LEVEL log_level, int log_sn);

public:
	list<string> logs_Pool_;				//日志缓存池
	pthread_mutex_t logs_Pool_lock_;		//日志缓存的锁

	unsigned int logs_current_normal_sn_;	//当前普通日志sn（序号）
	unsigned int logs_current_error_sn_;	//当前错误日志sn（序号）
	unsigned int logs_normal_written_byte;	//当前普通日志写入文件字节偏移
	unsigned int logs_error_written_byte;	//当前错误日志写入文件字节偏移

	bool logs_sync_flag_;					//log立即同步标志
	bool logs_zipped_flag_;					//log被压缩标志，0未压缩 1 被压缩  
	unsigned int logs_folder_size_;			//文件夹大小
};


#endif // !_QUICK_LOGGER_H_
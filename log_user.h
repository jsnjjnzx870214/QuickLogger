#ifndef _LOG_USER_H_
#define _LOG_USER_H_

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

extern int LogPrintf(LOG_LEVEL log_level, const char * tag, int color, const char * format, ...);



#endif

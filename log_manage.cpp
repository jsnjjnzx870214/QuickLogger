#include <string.h>
#include <memory>
#include <pthread.h>
#include "log_manage.h"
#include "quick_logger.h"

#ifdef __cplusplus
extern "C"
{
#endif

void * LogProess(void * ptr) {
	LogManager *log_manage_instant = (LogManager *)ptr;
	while (1) {
		timeval tv = { 0 };
		tv.tv_sec = 3;
		int nRet = select(0, NULL, NULL, NULL, &tv);
		if (nRet == 0) {	//timeout
			//printf("select timeout\n");
			log_manage_instant->LogWrite();
			log_manage_instant->LogSwitch();
			log_manage_instant->LogClean();
		}
	}
	return 0;
}


void Log_Manage() {
	LogManager& log_manage_instant = LogManager::GetInstance();

	int message_id = msgget(LOG_MESSAGE_QUEUE_KEY, IPC_CREAT | 0666);
	int message_receive_len = 0;
	ST_MESSAGE message = { 0 };

	pthread_t	logger_opera_id;

	if ((pthread_create(&logger_opera_id, NULL, LogProess, &log_manage_instant) == -1)) {
		printf("create error2!\n");
		return;
	}

	while (1)
	{
		message_receive_len = msgrcv(message_id, &message, sizeof(message.msg_text), 0x00/* | LOG_FEATURE_CODE*/, MSG_NOERROR);
		if (message_receive_len) {
			char * anchor = (char *)&message.msg_type;
			ST_LOGHEADER * log_head = (ST_LOGHEADER *)anchor;
			char * log_data_content = anchor + sizeof(ST_LOGHEADER);
			//printf("len :%d,sender:%x,type:%d,level:%d,length:%d\n", nRecvLen, pHead->sender,	pHead->type, pHead->level, pHead->length);
			if (LOG_TYPE_SYNC == log_head->type) {
				printf("进程号:%d立即同步\n", log_head->sender);
				log_manage_instant.m_logs_sync_flag = true;
			}
			else {
				//printf("进程号:%d收到日志%s\n", pHead->sender,pData);
				log_manage_instant.LogAppend((LOG_LEVEL)log_head->level, log_data_content);
			}
			memset(message.msg_text, 0x00, sizeof(message.msg_text));
		}
	}
	return ;
}
#ifdef __cplusplus
}
#endif






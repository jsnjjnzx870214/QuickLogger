#include <stdio.h>
#include <string.h>
#include <memory>
#include<pthread.h>
#include "basic_type.h"
#include "quick_logger.h"


void * LogProess(void * ptr) {
	LoggerManage * log_manage_instant = (LoggerManage *)ptr;
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
	LoggerManage log_manage_instant;
	int nMsqId = msgget(LOG_MESSAGE_QUEUE_KEY, IPC_CREAT | 0666);
	int nRecvLen = 0;
	ST_MESSAGE sMessage = { 0 };

	pthread_t	logger_opera_id;

	if ((pthread_create(&logger_opera_id, NULL, LogProess, &log_manage_instant) == -1)) {
		printf("create error2!\n");
		return;
	}

	while (1)
	{
		nRecvLen = msgrcv(nMsqId, &sMessage, sizeof(sMessage.msg_text), 0x00/* | LOG_FEATURE_CODE*/, MSG_NOERROR);
		if (nRecvLen) {
			char * pAnchor = (char *)&sMessage.msg_type;
			LogHeader * pHead = (LogHeader *)pAnchor;
			char * pData = pAnchor + sizeof(LogHeader);
			//printf("len :%d,sender:%x,type:%d,level:%d,length:%d\n", nRecvLen, pHead->sender,	pHead->type, pHead->level, pHead->length);
			if (LOG_TYPE_SYNC == pHead->type) {
				printf("进程号:%d立即同步\n", pHead->sender);
				log_manage_instant.logs_sync_flag_ = TRUE;
			}
			else {
				//printf("进程号:%d收到日志%s\n", pHead->sender,pData);
				log_manage_instant.LogAppend((LOG_LEVEL)pHead->level, pData);
			}
			memset(sMessage.msg_text, 0x00, sizeof(sMessage.msg_text));
		}
	}
	return ;

}


int main() {

	Log_Manage();
	while (1)
	{
		sleep(100);
	}
	return 0;
}





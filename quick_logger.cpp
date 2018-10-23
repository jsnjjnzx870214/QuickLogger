#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/types.h>
#include "quick_logger.h"


int GetFirstKeyPosition(const char* pStr, char nKey)
{
	for (unsigned int i = 0; i < strlen(pStr); i++) {
		if (pStr[i] == nKey)
			return i;
	}
	return -1;
}


int LogSend(LOG_TYPE log_type, LOG_LEVEL log_level, const char * pSendBuff, unsigned short nSendBuffLen)
{
	if (NULL == pSendBuff || 0 == nSendBuffLen) {
		return -1;
	}
		
	char * pData = new char[sizeof(LogHeader) + nSendBuffLen];
	if (NULL == pData) {
		printf("log-send apply for space err\n");
		return -2;
	}
	char * anchor = pData;
	LogHeader * pHead = (LogHeader*)anchor;
	
	pHead->message_type = MESSAGE_TYPE;
	pHead->sender = getpid();
	pHead->type = log_type;
	pHead->level = log_level;
	pHead->length = nSendBuffLen;
	memcpy(anchor + sizeof(LogHeader), pSendBuff, nSendBuffLen);
	int nMsqId = msgget(LOG_MESSAGE_QUEUE_KEY, IPC_CREAT | 0666);
	if (nMsqId < 0) {
		delete[]pData;
		return -3;
	}
	//printf("���̺�:%d������־%s\n", pHead->sender, pSendBuff);
	int nRet = msgsnd(nMsqId, pData, sizeof(LogHeader) + nSendBuffLen - sizeof(pHead->message_type), IPC_NOWAIT);
	if (nRet < 0){
		printf("log - send msgsnd err:%d",errno);
		delete[]pData;
		return -4;
	}
	delete[]pData;
	return 0;
}
//�ȼ�����־ͷ����ɫ���д�ӡ��Ȼ��ԭʼ���ķ�����Ϣ�����н��з���
int LogPrintf(LOG_LEVEL log_level, const char * tag, int color, const char * format, ...) {
	int nRet = -1;
	va_list ap;
	char prefix[40] = { 0 };
	struct timeval	tv;
	struct tm * p = NULL;
	int x,y;	//x ǰ������(ʱ�䡢pid��)   y ������������
	gettimeofday(&tv, NULL);
	p = localtime(&tv.tv_sec);
	x = snprintf(prefix, sizeof(prefix), "D %02d-%02d %02d:%02d:%02d.%03ld#%u~%s ", 1 + p->tm_mon, \
																					p->tm_mday, \
																					p->tm_hour, \
																					p->tm_min, \
																					p->tm_sec, \
																					tv.tv_usec / 1000, \
																					getpid(), \
																					tag);
	int nPosition = GetFirstKeyPosition(prefix, 'D');
	if (nPosition < 0) {
		return nRet = -1;
	}
	//printf("%s\n", prefix);
	prefix[nPosition] = (log_level == LOG_LEVEL_NORMAL) ? 'N' : ((log_level == LOG_LEVEL_DEBUG) ? 'D' : 'E');
	va_start(ap, format);
	y = vsnprintf(0, 0, format, ap);//Ԥ�������������ܳ���
	unsigned short nSendLen = x + y + 1;
	//printf("ͷ����:%d�����ݳ���:%d,���ٳ���:%d\n", x, y, nSendLen);
	char * pContent = new char[nSendLen];
	if (pContent == NULL){
		return nRet = -2;
	}
	memset(pContent, 0x00, sizeof(nSendLen));
	memcpy(pContent, prefix, x);
	vsnprintf(pContent + strlen(prefix), nSendLen - strlen(prefix), format, ap);
	va_end(ap);
	printf("\033[%dm%s\033[m\n", color, pContent);
	int i = strlen(pContent);
	pContent[i++] = '\n';
	pContent[i++] = '\0';

	if (log_level == LOG_LEVEL_DEBUG) {
		delete []pContent;
		return 0;
	}
	if (LogSend(LOG_TYPE_MESSAGE, log_level, pContent, strlen(pContent)) >= 0) {
		delete []pContent;
		return nRet = -3;
	}
	delete[]pContent;
	return nRet = 0;
}

LoggerManage::LoggerManage() {
	logs_zipped_flag_ = FALSE;
	logs_current_normal_sn_ = ReadCurrentLogSn(LOG_LEVEL_NORMAL);
	logs_current_error_sn_ = ReadCurrentLogSn(LOG_LEVEL_ERROR);
	printf("logs_current_normal_sn_:%d\nlogs_current_error_sn_:%d\n", logs_current_normal_sn_, logs_current_error_sn_);
	pthread_mutex_init(&logs_Pool_lock_, NULL);
}

LoggerManage::~LoggerManage() {

}

unsigned int LoggerManage::ReadCurrentLogSn(LOG_LEVEL log_level) {
	char filename[70] = { 0 };
	char tmp[10] = { 0 };
	unsigned int sn = 0;

	snprintf(filename, sizeof(filename), "%s%s",LOG_ROOT_PATH, (log_level== LOG_LEVEL_ERROR)? ERR_SN: NORMAL_SN);
	FILE * fp = fopen(filename, "r+b");
	if (NULL == fp) {
		fp = fopen(filename, "w+b");
		//�´�������Ŵ�1��ʼ
		snprintf(tmp, sizeof(tmp),"1\n");
		fputs(tmp, fp);
		printf("%s �����ɹ�\n", filename);
		fclose(fp);
		return 1;
	}
	fseek(fp, 0, SEEK_SET);
	fgets(tmp, sizeof(tmp), fp);
	if (strlen(tmp)) {
		sn = atoi(tmp);
	} else {
		sn = 0;
	}
	fclose(fp);
	return sn;
}

unsigned int LoggerManage::WriteCurrentLogSn(LOG_LEVEL log_level, int log_sn) {
	char filename[70] = { 0 };
	char tmp[10] = { 0 };

	snprintf(filename, sizeof(filename), "%s%s", LOG_ROOT_PATH, (log_level == LOG_LEVEL_ERROR) ? ERR_SN : NORMAL_SN);
	FILE * fp = fopen(filename, "w+b");
	if (NULL == fp) {
		return 0;
	}
	fseek(fp, 0, SEEK_SET);
	snprintf(tmp, sizeof(tmp), "%d\n", log_sn);
	fputs(tmp, fp);
	fclose(fp);
	return log_sn;
}

int LoggerManage::LogAppend(LOG_LEVEL log_level, const char* send_buff) {
	if (NULL == send_buff) {
		return -1;
	}
	pthread_mutex_lock(&logs_Pool_lock_);
	logs_Pool_.push_back(send_buff);
	pthread_mutex_unlock(&logs_Pool_lock_);
	if (log_level == LOG_LEVEL_ERROR) {
		char filename[70] = { 0 };
		snprintf(filename, sizeof(filename), LOG_ROOT_PATH "log-%07ld-all-error.txt", (long unsigned int)logs_current_error_sn_);
		FILE * fp = fopen(filename, "r+b");
		if (NULL == fp) {
			fp = fopen(filename, "w+b");
		}
		else {
			fseek(fp, 0, SEEK_END);
		}
		fputs(send_buff, fp);
		logs_error_written_byte = ftell(fp);
		fclose(fp);
	}
	return 0;
}

int LoggerManage::LogWrite() {
	char filename[70] = { 0 };
	if (logs_Pool_.size() >= MAX_LOG_CACHE_COUNT || logs_sync_flag_) {
		snprintf(filename, sizeof(filename), LOG_ROOT_PATH "log-%07ld-all-normal.txt", (long unsigned int)logs_current_normal_sn_);
		FILE * fp = fopen(filename, "r+b");
		if (NULL == fp) {
			fp = fopen(filename, "w+b");
		}
		else {
			fseek(fp, 0, SEEK_END);
		}
		pthread_mutex_lock(&logs_Pool_lock_);
		list< string >::iterator iter;
		for (iter = logs_Pool_.begin(); iter != logs_Pool_.end(); iter++) {
			fputs(iter->c_str(), fp);
		}
		logs_normal_written_byte = ftell(fp);
		printf("logs_normal_written_byte:%d\n", logs_normal_written_byte);
		logs_Pool_.clear();
		pthread_mutex_unlock(&logs_Pool_lock_);
		logs_sync_flag_ = FALSE;
		fclose(fp);
	}
	return 0;
}

int LoggerManage::LogSwitch() {
	if (logs_normal_written_byte >= LOG_SWITCH_BYTE) {
		printf("�л�NORMAL��־:%d->%d\n", logs_current_normal_sn_, logs_current_normal_sn_ +1);
		LogCompress(LOG_LEVEL_NORMAL, logs_current_normal_sn_);
		logs_current_normal_sn_ = (logs_current_normal_sn_ >= 9999999) ? 1 : logs_current_normal_sn_ + 1;
		logs_normal_written_byte = 0;
		WriteCurrentLogSn(LOG_LEVEL_NORMAL, logs_current_normal_sn_);
	} 
	if (logs_error_written_byte >= LOG_SWITCH_BYTE) {
		printf("�л�ERROR��־:%d->%d\n", logs_current_error_sn_, logs_current_error_sn_+1);
		LogCompress(LOG_LEVEL_ERROR, logs_current_error_sn_);
		logs_current_error_sn_ = (logs_current_error_sn_ >= 9999999) ? 1 : logs_current_error_sn_ + 1;
		logs_error_written_byte = 0;
		WriteCurrentLogSn(LOG_LEVEL_ERROR, logs_current_error_sn_);
	}
	return 0;
}

int LoggerManage::LogCompress(LOG_LEVEL log_level, unsigned int sn) {
	int ret = -1;
	char command[256] = { 0 };
	switch (log_level)
	{
	case LOG_LEVEL_NORMAL: {
		snprintf(command, sizeof(command), "cd " LOG_ROOT_PATH ";echo 'normal-log %u zipping';tar jcvf log-%07u-all-normal.tar.gz log-%07u-all-normal.txt;rm log-%07u-all-normal.txt", sn, sn, sn, sn);
		break;
	}
	case LOG_LEVEL_ERROR: {
		snprintf(command, sizeof(command), "cd " LOG_ROOT_PATH ";echo 'error-log %u zipping';tar jcvf log-%07u-all-error.tar.gz log-%07u-all-error.txt;rm log-%07u-all-error.txt", sn, sn, sn, sn);
		break;
	}
	default: {
		return -1;
		break;
	}
	}
	printf("tar command : %s\n", command);
	ret = system(command);
	if (ret != 0) {
		printf("system call err:%d\n", ret);
		return -1;
	}
	logs_zipped_flag_ = TRUE;
	return 0;
}

int LoggerManage::LogClean() {
#define FULL_NAME_LEN	80
#define ASCIC_LEN		128
	DIR * dir_ptr = NULL;
	struct dirent * file;
	struct stat stat_buf;
	char file_full_name[FULL_NAME_LEN] = { 0 };
	unsigned char log_type_normal_flag = FALSE, log_type_error_flag = FALSE;
	char ascic[ASCIC_LEN] = { 0 };
	unsigned int figure = 0;

	if (!logs_zipped_flag_) {
		return 0;
	}
	logs_zipped_flag_ = FALSE;

	if (logs_current_normal_sn_ > MAX_NORMAL_ZIP_LOGS_ON_DISK) {
		printf("logs_current_normal_sn_ need clean\n");
		log_type_normal_flag = TRUE;
	}
	if (logs_current_error_sn_ > MAX_ERROR_ZIP_LOGS_ON_DISK) {
		printf("logs_current_error_sn_ need clean\n");
		log_type_error_flag = TRUE;
	}
	if (!log_type_normal_flag && !log_type_error_flag) {
		printf("no log need to be clean\n");
	}

	dir_ptr = opendir(LOG_ROOT_PATH);
	if (NULL == dir_ptr) {
		printf("opendir err!\n");
		return -1;
	}

	logs_folder_size_ = 0;
	while ((file = readdir(dir_ptr)) != NULL) {
		
		if (file->d_type != DT_REG) {
			continue;	//ȥ���ǳ����ļ�
		}
		memset(file_full_name, 0x00, FULL_NAME_LEN);
		snprintf(file_full_name, FULL_NAME_LEN, LOG_ROOT_PATH "%s", file->d_name);
		stat(file_full_name, &stat_buf);
		logs_folder_size_ += stat_buf.st_size;
		//printf("file name is %s,size is %d\n", file->d_name, (int)stat_buf.st_size);
		//���ж�normal�ļ�
		if (log_type_normal_flag && (0 != strstr(file->d_name,"normal.tar.gz"))) {
			//log-xxxxxx-all-normal.tar.gz
			figure = atoi(&file->d_name[4]);
			if (figure <= logs_current_normal_sn_ - MAX_NORMAL_ZIP_LOGS_ON_DISK - 1) {
				printf("file:%s need to be delete\n", file->d_name);
				snprintf(ascic, ASCIC_LEN,"cd " LOG_ROOT_PATH "; rm %s", file->d_name);
				system(ascic);
			}
		}
		//���ж�error�ļ�
		if (log_type_error_flag && (0 != strstr(file->d_name, "error.tar.gz"))) {
			//log-xxxxxx-all-normal.tar.gz
			figure = atoi(&file->d_name[4]);
			if (figure <= logs_current_error_sn_ - MAX_ERROR_ZIP_LOGS_ON_DISK - 1) {
				printf("file:%s need to be delete\n", file->d_name);
				snprintf(ascic, ASCIC_LEN, "cd " LOG_ROOT_PATH "; rm %s", file->d_name);
				system(ascic);
			}
		}
	}
	closedir(dir_ptr);
	//�ռ��ļ��д�С����
	if (logs_folder_size_ > MAX_LOG_DIR_SPACE) {
		system("rm " LOG_ROOT_PATH "/*;");
		printf("�ļ��г����趨��ֵ��ɾ��ȫ������\n");
	}

	return 0;
}








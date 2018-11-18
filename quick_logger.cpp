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


int GetFirstKeyPosition(const char* pStr, char nKey) {
	for (unsigned int i = 0; i < strlen(pStr); i++) {
		if (pStr[i] == nKey)
			return i;
	}
	return -1;
}

int LogSend(LOG_TYPE log_type, LOG_LEVEL log_level, const char * pSendBuff, unsigned short nSendBuffLen) {
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
	//printf("进程号:%d发送日志%s\n", pHead->sender, pSendBuff);
	int nRet = msgsnd(nMsqId, pData, sizeof(LogHeader) + nSendBuffLen - sizeof(pHead->message_type), IPC_NOWAIT);
	if (nRet < 0){
		printf("log - send msgsnd err:%d",errno);
		delete[]pData;
		return -4;
	}
	delete[]pData;
	return 0;
}
//先加上日志头和颜色进行打印，然后将原始报文放入消息队列中进行发送
int LogPrintf(LOG_LEVEL log_level, const char * tag, int color, const char * format, ...) {
	int nRet = -1;
	va_list ap;
	char prefix[40] = { 0 };
	struct timeval	tv;
	struct tm * p = NULL;
	int x,y;	//x 前导长度(时间、pid等)   y 不定参数长度
	gettimeofday(&tv, NULL);
	p = localtime(&tv.tv_sec);
	x = snprintf(prefix, sizeof(prefix), "D %02d-%02d %02d:%02d:%02d.%03ld#%u~%s ", (1 + p->tm_mon) > 12 ? (1 + p->tm_mon) : 1, \
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
	y = vsnprintf(0, 0, format, ap);//预估不定参数的总长度
	unsigned short nSendLen = x + y + 1;
	//printf("头长度:%d，内容长度:%d,开辟长度:%d\n", x, y, nSendLen);
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

LogManager* LogManager::log_manage_instance_ = NULL;

LogManager::LogManager() {
	logs_zipped_flag_ = false;
	logs_pool_total_len = 0;
	logs_current_normal_sn_ = ReadCurrentLogSn(LOG_LEVEL_NORMAL);
	logs_current_error_sn_ = ReadCurrentLogSn(LOG_LEVEL_ERROR);
	printf("logs_current_normal_sn_:%d\nlogs_current_error_sn_:%d\n", logs_current_normal_sn_, logs_current_error_sn_);
}

LogManager& LogManager::GetInstance() {
	if (log_manage_instance_ == NULL) {
		log_manage_instance_ = new LogManager();
	}
	return *log_manage_instance_;
}

unsigned int LogManager::ReadCurrentLogSn(LOG_LEVEL log_level) {
	char filename[70] = { 0 };
	char tmp[10] = { 0 };
	unsigned int sn = 0;

	snprintf(filename, sizeof(filename), "%s%s", LOG_ROOT_PATH, (log_level == LOG_LEVEL_ERROR) ? ERR_SN : NORMAL_SN);
	FILE * fp = fopen(filename, "r+b");
	if (NULL == fp) {
		fp = fopen(filename, "w+b");
		//新创建，序号从1开始
		snprintf(tmp, sizeof(tmp), "1\n");
		fputs(tmp, fp);
		printf("%s 创建成功\n", filename);
		fclose(fp);
		return 1;
	}
	fseek(fp, 0, SEEK_SET);
	fgets(tmp, sizeof(tmp), fp);
	if (strlen(tmp)) {
		sn = atoi(tmp);
	}
	else {
		sn = 0;
	}
	fclose(fp);
	return sn;
}

unsigned int LogManager::WriteCurrentLogSn(LOG_LEVEL log_level, unsigned int log_sn) {
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


int LogManager::LogAppend(LOG_LEVEL log_level, const char* send_buff) {
	if (NULL == send_buff) {
		return -1;
	}
	pthread_mutex_lock(&logs_pool_lock_);
	logs_pool_.push_back(send_buff);
	logs_pool_total_len += strlen(send_buff);
	pthread_mutex_unlock(&logs_pool_lock_);

	if (logs_pool_total_len >= MAX_LOG_POOL_BYTES) {
		logs_pool_total_len = 0;
		logs_sync_flag_ = true;
	}

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
		logs_error_written_byte_ = ftell(fp);
		fclose(fp);
	}
	return 0;
}

int LogManager::LogWrite() {
	char filename[70] = { 0 };
	if (logs_pool_.size() >= MAX_LOG_CACHE_COUNT || logs_sync_flag_) {
		snprintf(filename, sizeof(filename), LOG_ROOT_PATH "log-%07ld-all-normal.txt", (long unsigned int)logs_current_normal_sn_);
		FILE * fp = fopen(filename, "r+b");
		if (NULL == fp) {
			fp = fopen(filename, "w+b");
		}
		else {
			fseek(fp, 0, SEEK_END);
		}
		pthread_mutex_lock(&logs_pool_lock_);
		list< string >::iterator iter;
		for (iter = logs_pool_.begin(); iter != logs_pool_.end(); iter++) {
			fputs(iter->c_str(), fp);
		}
		logs_normal_written_byte_ = ftell(fp);
		printf("logs_normal_written_byte:%d\n", logs_normal_written_byte_);
		logs_pool_.clear();
		pthread_mutex_unlock(&logs_pool_lock_);
		logs_sync_flag_ = false;
		fclose(fp);

	}
	return 0;
}

int LogManager::LogSwitch() {
	if (logs_normal_written_byte_  >= LOG_SWITCH_BYTE) {
		printf("切换NORMAL日志:%d->%d\n", logs_current_normal_sn_, logs_current_normal_sn_ + 1);
		LogCompress(LOG_LEVEL_NORMAL, logs_current_normal_sn_);
		logs_current_normal_sn_ = logs_current_normal_sn_ + 1;
		logs_normal_written_byte_ = 0;
		WriteCurrentLogSn(LOG_LEVEL_NORMAL, logs_current_normal_sn_);
	}
	if (logs_error_written_byte_ >= LOG_SWITCH_BYTE) {
		printf("切换ERROR日志:%d->%d\n", logs_current_error_sn_, logs_current_error_sn_ + 1);
		LogCompress(LOG_LEVEL_ERROR, logs_current_error_sn_);
		logs_current_error_sn_ = logs_current_error_sn_ + 1;
		logs_error_written_byte_ = 0;
		WriteCurrentLogSn(LOG_LEVEL_ERROR, logs_current_error_sn_);
	}
	return 0;
}

int LogManager::LogCompress(LOG_LEVEL log_level, unsigned int sn) {
	int ret = -1;
	char command[256] = { 0 };
	switch (log_level)
	{
	case LOG_LEVEL_NORMAL: {
		snprintf(command, sizeof(command), "cd " LOG_ROOT_PATH ";tar jcvf log-%07u-all-normal.tar.gz log-%07u-all-normal.txt;rm log-%07u-all-normal.txt", sn, sn, sn);
		break;
	}
	case LOG_LEVEL_ERROR: {
		snprintf(command, sizeof(command), "cd " LOG_ROOT_PATH ";tar jcvf log-%07u-all-error.tar.gz log-%07u-all-error.txt;rm log-%07u-all-error.txt", sn, sn, sn);
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
	logs_sync_flag_ = true;
	logs_zipped_flag_ = true;
	return 0;
}

int LogManager::LogClean() {
#define FULL_NAME_LEN	128
#define ASCIC_LEN		80
	DIR * dir_ptr = NULL;
	struct dirent * file;
	struct stat stat_buf;
	char file_full_name[FULL_NAME_LEN] = { 0 };
	char ascic[ASCIC_LEN] = { 0 };
	unsigned int figure = 0;
	unsigned int logs_folder_size = 0;
	if (!logs_zipped_flag_) {
		return 0;
	}
	logs_zipped_flag_ = false;

	dir_ptr = opendir(LOG_ROOT_PATH);
	if (NULL == dir_ptr) {
		printf("opendir err!\n");
		return -1;
	}
	while ((file = readdir(dir_ptr)) != NULL) {

		if (file->d_type != DT_REG) {
			continue;	//去除非常规文件
		}
		memset(file_full_name, 0x00, FULL_NAME_LEN);
		snprintf(file_full_name, FULL_NAME_LEN, LOG_ROOT_PATH "%s", file->d_name);
		stat(file_full_name, &stat_buf);
		logs_folder_size += stat_buf.st_size;
		//printf("file name is %s,size is %d\n", file->d_name, (int)stat_buf.st_size);
		//先判断normal文件
		if (0 != strstr(file->d_name, "normal.tar.gz") || 0 != strstr(file->d_name, "normal.txt")) {
			//log-xxxxxx-all-normal.tar.gz
			figure = atoi(&file->d_name[4]);
			printf("file name is %s,figure :%d  logs_current_normal_sn_:%d\n", file->d_name, figure, logs_current_normal_sn_);
			if ((logs_current_normal_sn_ > MAX_NORMAL_ZIP_LOGS_ON_DISK) && (figure <= logs_current_normal_sn_ - MAX_NORMAL_ZIP_LOGS_ON_DISK - 1)) {
				printf("file:%s need to be delete\n", file->d_name);
				snprintf(ascic, ASCIC_LEN, "cd " LOG_ROOT_PATH "; rm %s", file->d_name);
				system(ascic);
			}
		}
		//再判断error文件
		if (0 != strstr(file->d_name, "error.tar.gz") || 0 != strstr(file->d_name, "error.txt")) {
			//log-xxxxxx-all-normal.tar.gz
			figure = atoi(&file->d_name[4]);
			if ((logs_current_error_sn_ > MAX_ERROR_ZIP_LOGS_ON_DISK) && (figure <= logs_current_error_sn_ - MAX_ERROR_ZIP_LOGS_ON_DISK - 1)) {
				printf("file:%s need to be delete\n", file->d_name);
				snprintf(ascic, ASCIC_LEN, "cd " LOG_ROOT_PATH "; rm %s", file->d_name);
				system(ascic);
			}
		}
	}
	closedir(dir_ptr);
	//终极文件夹大小防护
	if (logs_folder_size > MAX_LOG_DIR_SPACE) {
		system("rm " LOG_ROOT_PATH "/*;");
		printf("文件夹超出设定阈值，删除全部内容\n");
	}

	return 0;
}





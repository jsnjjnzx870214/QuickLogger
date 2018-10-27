#include "quick_logger.h"


void * Task1(void * pData) {
	while (1) {
		LogN_R("task1", "Hello %s", "test_task1 1");
		usleep(100000);
		LogN_G("task1", "Hello %s", "test_task1 2");
		usleep(150000);
		LogN_Y("task1", "Hello %s", "test_task1 3");
		usleep(200000);
		LogN_B("task1", "Hello %s", "test_task1 4");
		usleep(250000);
		LogN_P("task1", "Hello %s", "test_task1 5");
		usleep(300000);
		LogN_C("task1", "Hello %s", "test_task1 6");
		usleep(350000);
		LogN_W("task1", "Hello %s", "test_task1 7");
		usleep(654321);
	}
	return NULL;
}

void * Task2(void * pData) {
	while (1) {
		LogN_R("task2", "Hello %s", "test_task2 1");
		usleep(100000);
		LogN_G("task2", "Hello %s", "test_task2 2");
		usleep(180000);
		LogN_Y("task2", "Hello %s", "test_task2 3");
		usleep(260000);
		LogN_B("task2", "Hello %s", "test_task2 4");
		usleep(340000);
		LogN_P("task2", "Hello %s", "test_task2 5");
		usleep(420000);
		LogN_C("task2", "Hello %s", "test_task2 6");
		usleep(500000);
		LogN_W("task2", "Hello %s", "test_task2 7");
		usleep(596476);
	}
	return NULL;
}

void * Task3(void * pData) {
	while (1) {
		LogN_R("task3", "Hello %s", "test_task3 1");
		usleep(100220);
		LogE_G("task3", "Hello %s", "test_task3 2");
		usleep(258000);
		LogN_Y("task3", "Hello %s", "test_task3 3");
		usleep(214000);
		LogE_B("task3", "Hello %s", "test_task3 4");
		usleep(457100);
		LogN_P("task3", "Hello %s", "test_task3 5");
		usleep(567700);
		LOGE_C("task3", "Hello %s", "test_task3 6");
		usleep(111100);
		LogN_W("task3", "Hello %s", "test_task3 7");
		usleep(654321);
	}
	return NULL;
}

void * Task4(void * pData) {
	while (1) {
		LogD_R("task4", "Hello %s", "test_task4 1");
		usleep(100000);
		LogN_G("task4", "Hello %s", "test_task4 2");
		usleep(150000);
		LogN_Y("task4", "Hello %s", "test_task4 3");
		usleep(200000);
		LogD_B("task4", "Hello %s", "test_task4 4");
		usleep(250000);
		LogN_P("task4", "Hello %s", "test_task4 5");
		usleep(300000);
		LOGD_C("task4", "Hello %s", "test_task4 6");
		usleep(350000);
		LogN_W("task4", "Hello %s", "test_task4 7");
		usleep(654321);
	}
	return NULL;
}

int main() {

	pthread_t	logger_task1_id, logger_task2_id, logger_task3_id, logger_task4_id;

	if ((pthread_create(&logger_task1_id, NULL, Task1, NULL) == -1)) {
		printf("create error1!\n");
		return 0;
	}
	if ((pthread_create(&logger_task2_id, NULL, Task2, NULL) == -1)) {
		printf("create error2!\n");
		return 0;
	}
	if ((pthread_create(&logger_task3_id, NULL, Task3, NULL) == -1)) {
		printf("create error3!\n");
		return 0;
	}
	if ((pthread_create(&logger_task4_id, NULL, Task4, NULL) == -1)) {
		printf("create error4!\n");
		return 0;
	}
	while (1) {
		sleep(1);
	}
	return 0;
}
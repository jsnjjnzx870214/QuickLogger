#include "quick_logger.h"

int main() {
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
}
#include "quick_logger.h"

int main() {
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
}
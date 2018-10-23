#include "quick_logger.h"

int main() {
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
}
#include "quick_logger.h"

int main() {
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
}
#include <unistd.h>
#include "log_manage.h"

int main() {
	Log_Manage();
	while (1) {
		sleep(10000);
	}
	return 0;
}
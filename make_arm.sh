#!/bin/bash


rm *.o;
rm *.so;
# 编译库
arm-none-linux-gnueabi-g++ -O0 -Wall -c quick_logger.cpp -o quick_logger.o
arm-none-linux-gnueabi-g++ -o libquick_logger.so quick_logger.o -lpthread -shared -fPIC

# 编译管理进程
arm-none-linux-gnueabi-g++ -O0 -Wall -c log_manage.cpp -o log_manage.o
arm-none-linux-gnueabi-g++ -o manage log_manage.o -lpthread -L . -lquick_logger -Wl,--rpath=.

# 编译发送测试进程1
arm-none-linux-gnueabi-g++ -O0 -Wall -c test_task.cpp -o test_task.o
arm-none-linux-gnueabi-g++ -o log_task test_task.o -lpthread -L . -lquick_logger -Wl,--rpath=.



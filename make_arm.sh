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
arm-none-linux-gnueabi-g++ -O0 -Wall -c test_task1.cpp -o test_task1.o
arm-none-linux-gnueabi-g++ -o task1 test_task1.o -lpthread -L . -lquick_logger -Wl,--rpath=.

# 编译发送测试进程2
arm-none-linux-gnueabi-g++ -O0 -Wall -c test_task2.cpp -o test_task2.o
arm-none-linux-gnueabi-g++ -o task2 test_task2.o -lpthread -L . -lquick_logger -Wl,--rpath=.

# 编译发送测试进程3
arm-none-linux-gnueabi-g++ -O0 -Wall -c test_task3.cpp -o test_task3.o
arm-none-linux-gnueabi-g++ -o task3 test_task3.o -lpthread -L . -lquick_logger -Wl,--rpath=.

# 编译发送测试进程4
arm-none-linux-gnueabi-g++ -O0 -Wall -c test_task4.cpp -o test_task4.o
arm-none-linux-gnueabi-g++ -o task4 test_task4.o -lpthread -L . -lquick_logger -Wl,--rpath=.



#!/bin/bash


rm *.o;
rm *.so;

# 编译库
g++ -O0 -Wall -c quick_logger.cpp -o quick_logger.o
g++ -O0 -Wall -c log_manage.cpp -o log_manage.o
g++ -lpthread -shared -fPIC -o libquick_logger.so quick_logger.o log_manage.o
echo "libquick_logger.so build ok"

# 编译管理进程
gcc -O0 -Wall -c main.c -o main.o
gcc -o manage main.o  -L . -lquick_logger -Wl,--rpath=.  -lpthread
echo "manage build ok"

# 编译发送测试进程
g++ -O0 -Wall -c test_task.cpp -o test_task.o
g++ -o log_task test_task.o  -L . -lquick_logger -Wl,--rpath=. -lpthread
echo "log_task build ok"





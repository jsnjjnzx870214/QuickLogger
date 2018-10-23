# QuickLogger

---

# 1. 介绍

[QuickLogger](https://github.com/jsnjjnzx870214/QuickLogger)是一款应用于嵌入式Linux设备（Embedded Linux）中的快速日志管理程序。

## 1.1 主要特性

- 充分考虑了嵌入式Linux系统中flash的特性，具备定时存储，批量存储等多种功能（考虑flash寿命）；
- 结合设备Debug功能，将日志分为多个层次，满足现场调试和日志纠错的需求；
- 日志内容包含级别、时间戳、线程（进程）信息等；
- 日志输出被设计为线程安全的方式；
- 具备按照设定要求，定时备份日志和清理老的日志功能；
- 结合基本的正则表达式，支持按 **标签**  、 **级别** 、 **关键词** 、**进程号** 等进行动态过滤；
- 各标签日志支持不同颜色显示；
- 充分考虑了嵌入式设备的供电可靠性问题，具备掉电后上电日志系统正常保护功能；

## 1.2 设计思路
### 由于嵌入式Linux终端一般不会很复杂，且程序员大多数也搞不清具体某日志到底属于哪个日志层级，所以摒弃了谷歌安卓五级日志层级，改为三级日志层级



- 0.[D]：调试(Debug) 	LOG-D   仅打印，不保存         保存普通流程打印信息


- 1.[V]：普通(Normal)   LOG-V   打印，定时保存至flash	 保存需要定期观察的打印信息（缓存写入，可能会丢失）


- 2.[E]：错误(Error) 	LOG-E   打印，立即保存至flash	 保存致命错误打印信息（立即写入，不会丢失）

> 注：谷歌Android Logcat日志层级 ，级别最高为 0(Assert) ，最低为 5(Verbose) 。

- 0.[A]：断言(Assert)

- 1.[E]：错误(Error)

- 2.[W]：警告(Warn)

- 3.[I]：信息(Info)

- 4.[D]：调试(Debug)

- 5.[V]：详细(Verbose)


### 流程（以LOG-V 为例）：
#### 1.扫描日志文件夹内所有日志文件，里面可能包含以下几种类型：

- log-xxxxxxx-all-normal.tar.gz		已经保存并压缩的普通日志；
- log-xxxxxxx-all-normal.txt		正在写入的普通日志；
- log-xxxxxxx-all-error.tar.gz		已经保存并压缩的致命日志；
- log-xxxxxxx-all-error.txt			正在写入的致命日志；

#### 2.查找是否有LOG-V-xxxxxxx.txt，如果有，则表示非第一次写入日志，准备进行追加写入。

#### 3.追加写入前，检查该txt文件是否超过指定大小（可配置），如超过，则进行压缩，并创建下一个txt文件，若没有超过，则追加写入。

#### 4.若上一步创建了一个新的txt文件，则把前一个文件关闭，并检查同级别log-xxxxxxx-all-normal.tar.gz的数量，若超过指定数量（可配置），则删除多余的压缩包（多余多少可配置）。

> 注：按每分钟产生一个txt文件并压缩，7位数可无间断保存9999999/1440/356=19年，完全满足需要。


## 1.3 Star & Fork

后续我还会根据大家需求，不断的完善和测试新代码。也非常欢迎大家设计、开发更多实用插件和功能，一起来完善 QuickLogger 。如果觉得这个开源项目很赞，可以点击[项目主页](https://github.com/jsnjjnzx870214/QuickLogger) 右上角的 **Star** ，同时把它推荐给更多有需要的朋友。

# 2. 使用

## 2.1 基本API
- Log_Manage 	日志管理线程;
- LogProess  	日志处理数据线程;
- LogWrite		日志写入;
- LogSwitch		日志切换；
- LogClean		日志清理；

## 2.2 输出级别
- LogD			Debug日志级别
- LogN			Normal日志级别
- LogE			Error日志级别

## 2.3 多彩日志
**只需要在LogD、LogN、LogE后增加_R、_G等，即可增加多种颜色的日志，内置颜色七种，分别如下：**

- LogD_R		红色日志
- LogD_G		绿色日志
- LogD_Y		黄色日志
- LogD_B		蓝色日志
- LogD_P		粉色日志
- LogD_C		蓝绿色日志
- LogD_W		白色日志

## 2.4 输出过滤

**目前输出过滤功能考虑用编辑器实现，本软件内暂不实现（好像没有多大的必要性，用编辑器配合正则表达式可以很好的实现日志过滤，且更为灵活）**

## 2.5 输出格式

**有两种输出格式：**

- 第一种调用大小写LogD，输出：
N 10-13 20:56:11.106#895~task1 Hello test_task1

- 第二种调用大小写LOGD，输出：
N 10-13 20:56:11.106#895~task1 Hello test_task1  <test_task1.cpp:15>

## 2.6 Demo

**请查看示例程序 log_manage.cpp**

您只需开启一个进程，调用一次Log_Manage()后，即可完成日志的管理工作。其他任何进程或线程，直接调用宏（类似LogD），即可

![运行示意](https://i.imgur.com/yYx1vYU.gif)
# 3. 使用注意事项
**使用期请先配置log_config.h文件，说明如下：**

```
#define MESSAGE_TYPE				300356			//不知道写啥，就写公司股票代码吧 
#define LOG_MESSAGE_QUEUE_KEY		300356			//日志消息队列KEY
#define LOG_SWITCH_BYTE				1024 * 256		//日志切换最大字节数  
#define MAX_NORMAL_ZIP_LOGS_ON_DISK	50				//普通日志压缩包保留数量
#define MAX_ERROR_ZIP_LOGS_ON_DISK	5				//错误日志压缩包保留数量
#define MAX_LOG_CACHE_COUNT			50				//普通日志最大缓存数量，超出此值，写入FLASH
#define MAX_LOG_DIR_SPACE			1024*1024*10	//日志目录最大给定空间，用于发生致命错误时，保护终端存储空间不会被占满

#define LOG_ROOT_PATH		"/mnt/user/LOG/"		//日志目录
#define ERR_SN				"sn-error-record.txt"	//错误日志的序号文件
#define NORMAL_SN			"sn-normal-record.txt"	//一般日志的序号文件
```

# 4. 后期

# 5. 许可

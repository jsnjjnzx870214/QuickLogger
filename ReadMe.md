# QuickLogger

---

# 1. 介绍

[QuickLogger](https://github.com/jsnjjnzx870214/QuickLogger)是一款应用于嵌入式Linux设备（Embedded Linux）中的快速日志管理包，

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


- 1.[V]：详细(Verbose)   LOG-V   打印，定时保存至flash	 保存需要定期观察的打印信息（缓存写入，可能会丢失）


- 2.[E]：致命(Error) 	LOG-E   打印，立即保存至flash	 保存致命错误打印信息（立即写入，不会丢失）

> 注：谷歌Android Logcat日志层级 ，级别最高为 0(Assert) ，最低为 5(Verbose) 。

- 0.[A]：断言(Assert)

- 1.[E]：错误(Error)

- 2.[W]：警告(Warn)

- 3.[I]：信息(Info)

- 4.[D]：调试(Debug)

- 5.[V]：详细(Verbose)


### 流程（以LOG-V 为例）：
#### 1.扫描日志文件夹内所有日志文件，里面可能包含以下几种类型：

- LOG-V-0000001.tar.gz	已经保存并压缩的普通日志；
- LOG-V-0000001.txt		正在写入的普通日志；
- LOG-E-0000001.tar.gz	已经保存并压缩的致命日志；
- LOG-E-0000001.txt		正在写入的致命日志；
#### 2.查找是否有LOG-V-xxxxxxx.txt，如果有，则表示非第一次写入日志，准备进行追加写入。

#### 3.追加写入前，检查该txt文件是否超过指定大小（可配置），如超过，则进行压缩，并创建下一个txt文件，若没有超过，则追加写入。

#### 4.若上一步创建了一个新的txt文件，则把前一个文件关闭，并检查同级别LOG-V-0000001.tar.gz的数量，若超过指定数量（可配置），则删除序号最小的一个。

> 注：按每分钟产生一个txt文件并压缩，7位数可无间断保存9999999/1440/356=19年，完全满足需要。


## 1.3 Star & Fork

后续我还会根据大家需求，不断的完善和测试新代码。也非常欢迎大家设计、开发更多实用插件和功能，一起来完善 QuickLogger 。如果觉得这个开源项目很赞，可以点击[项目主页](https://github.com/jsnjjnzx870214/QuickLogger) 右上角的 **Star** ，同时把它推荐给更多有需要的朋友。

# 2. 使用

## 2.1 基本API
- 日志初始化函数`log_Initialize`;
- 日志注册函数`log_Register`;
- 日志输出：`LOGD` `LOGV` `LOGE`;

## 2.2 输出级别

## 2.3 多彩日志

## 2.4 输出过滤

## 2.5 输出格式

## 2.6 输出方式

## 2.7 Demo

# 3. 使用注意事项

# 4. 后期

# 5. 许可

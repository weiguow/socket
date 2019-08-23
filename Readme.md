# socket
* 本项目要求：\
通过socket通信，客户端请求服务端，支持多人同时“增删改查”图书信息，具备以下功能：\
	A. 图书信息（书名，读者，简介）存储，采用某种数据结构来存储图书信息；\
	B. 可以多人在线操作图书内容，注意同步和异步，需要加锁处理；\
	C. 并采用数据结构来存储和支持查找功能，看看怎么处理可以比较高效；\
	D. 建议两三个人合作实现，注意下编码风格和交互的接口。

---
## 如何使用
* 前提条件：linux系统下，g++版本支持c++11，支持make

* 本地运行：
1. 在本文件夹根目录下，运行make_client.sh，即可编译出可执行文件client
```shell
./make_client.sh
```
2. 运行make，编译出服务器可执行文件output/server.1.0.0
```shell
make
```
3. 在output文件夹下创建数据文件
```shell
cd output/
touch data.db
```
4. 执行服务器端，开启服务器
```shell
./server.1.0.0
```
5. 在另一个终端中到本文件夹根目录下，运行客户端即可开启与服务器通信
```shell
./client
```

* 公网运行
1. 在本文件夹根目录下，运行make_client.sh，即可编译出可执行文件client
```shell
./make_client.sh
```
2. 运行client客户端，并且输入公网iq和端口号。
```shell
./client 118.89.135.145 8150
```
---
## 用户操作
当连接成功时，客户端会出现以下文字：
```shell
请选择操作编号
1.新增图书
2.删除图书
3.改动图书
4.查询图书
输q退出操作
输入操作编号：
```
用户需要在终端上输入操作编号（即1、2、3、4或q）即可进行下一步操作。

---
## 文件说明
```
.\
├── client                                      // 可执行文件client端
├── echo_clnt2.cpp                              // Client端文件名
├── include                                     // server端头文件文件夹
│   ├── Book.h                                  // 数据结构类和Book类封装
│   ├── CTimer.h                                // 定时类头文件
│   ├── file_operations.h                       // 定义文件操作的头文件
│   ├── json                                    // json库头文件文件夹
│   │   ├── json-forwards.h                     // json文件
│   │   └── json.h                              // json库头文件
│   ├── Operation.h                             // 操作类头文件
│   └── Thread.h                                // 线程池头文件
├── jsoncpp.cpp                                 // json库文件
├── make_client.sh                              // 生成client端可执行文件的脚本
├── Makefile                                    // 用于生成server端可执行文件的makefile文件
├── output                                      // 用于输出make结果的文件夹
│   ├── data.db                                 // 本地化存储的文件
│   └── server.1.0.0                            // server端可执行文件
└── src                                         // 用于存放server端的编译文件        
    ├── Book.cpp                                // 数据结构类和Book类封装文件
    ├── CTimer.cpp                              // 定时类文件
    ├── edge_trigger_epoll_serv.cpp             // server端主文件
    ├── file_operations.cpp                     // 文件操作
    ├── jsoncpp.cpp                             // json操作文件
    ├── Operation.cpp                           // server端的操作代码
    └── Thread.cpp                              // 线程池封装代码
```
---
## 测试结果
待录入

---
## 运行截图
待录入

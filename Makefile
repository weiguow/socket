# CC = gcc
# CXX = g++
# CFLAGS = -O -DDBG -D_REENTRANT -Wall -DUSE_PTHREAD -pthread -g -Wall -std=c++11
# LDFLAGS = -ldl -lnsl -lm -lpthread
 
# CPP_SRC = $(wildcard *.cpp)  
# CPP_OBJ = $(patsubst %cpp, %o, $(CPP_SRC))  

# SOURCE = $(wildcard *.cpp)
# TARGETS = $(patsubst %.cpp, %, $(SOURCE))
# .PHONY:all clean  
  
# all:$(TARGETS)

# $(TARGETS):%:%.cpp  
# 	$(CXX) $< $(CFLAGS) -o $@  
  
  
# clean:  
# 	rm *~ *.o -f  

VERSION = 1.0.0		#程序版本号
 
SOURCE = $(wildcard ./src/*.cpp)	#获取所有的.cpp文件
OBJ = $(patsubst %.cpp, %.o, $(SOURCE))	#将.cpp文件转为.o文件
INCLUDES = -I./include/	#头文件路径
 
# LIBS = -ldylib		#库文件名字
# LIB_PATH = -L./lib	#库文件地址
 
# DEBUG = -D_MACRO	#宏定义
CFLAGS = -Wall -c -std=c++11 	#编译标志位
 
TARGET = app
# CC = gcc
CXX = g++
 
$(TARGET): $(OBJ)	
	@mkdir -p output/	#创建一个目录，用于存放已编译的目标
	$(CXX) $(OBJ) -o output/$(TARGET).$(VERSION) -lpthread
 
%.o:%.cpp
	$(CXX) $(INCLUDES) $(CFLAGS) $< -o $@ -lpthread
 
.PHONY: clean
clean:
	rm -rf $(OBJ) output/ 
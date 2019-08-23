### 
# @Author: Liangyuhang
 # @LastEditors: Liangyuhang
 ###
#!bin/sh
g++ -c jsoncpp.cpp -o jsoncpp.o
g++ -std=c++11 -c echo_clnt2.cpp -o echo_clnt2.o
g++ echo_clnt2.o jsoncpp.o -o client
rm -f *.o
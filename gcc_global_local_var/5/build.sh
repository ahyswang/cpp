#!/bin/sh


g++ -fPIC -c -o libAMath.o libAMath.cpp
g++ -fPIC -c -o libBMath.o libBMath.cpp
ar -rv libAMath.a libAMath.o
ar -rv libBMath.a libBMath.o
# 全局变量默认是对外可见改为默认不可见
# -Wl,--exclude-libs,ALL  链接器将静态库中的所有符号转换为隐藏
#g++ -fPIC libA.cpp -shared -fvisibility=hidden -Wl,--exclude-libs,ALL -o libA.so -L./ -lAMath
g++ -fPIC libA.cpp -shared -o libA.so -L./ -lAMath
#g++ -fPIC -Wl,-Bsymbolic  libA.cpp -shared -o libA.so -L./ -lAMath
# 配置编译导出符号的，不起作用
#g++ -fPIC libA.cpp -shared -fvisibility=hidden -Wl,--exclude-libs,ALL -Wl,--retain-symbols-file=libA.sym -Wl,--version-script=libA.map -o libA.so -L./ -lAMath


#g++ -fPIC libB.cpp -shared -fvisibility=hidden -Wl,--exclude-libs,ALL -o libB.so -L./ -lBMath
g++ -fPIC libB.cpp -shared -o libB.so -L./ -lBMath
#g++ -fPIC -Wl,-Bsymbolic libB.cpp -shared -o libB.so -L./ -lBMath

#g++ main.cpp libA.so libB.so -o main

g++ main.cpp  libA.so libB.so -o main

export LD_LIBRARY_PATH=.

./main

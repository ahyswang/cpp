## 说明


## 步骤 

### 编译

$(CXX) -Wl,-Map=test.map -g test.cpp -o test

### 查找divide函数的地址

grep divide test.map
```
0x000000000040063d
```

### 使用addr2line定位
addr2line 0x000000000040063d -e test -f -C -s
```
divide(int, int)
test.cpp:4
```

### 查找系统信息，然后定位代码

dmesg | grep test
```
[6822291.865352] traps: test[2006] trap divide error ip:40064b sp:7ffde5d17ef0 error:0 in test[400000+1000]
```

这条信息里，ip（指令指针寄存器）字段后面的数字就是test程序出错时程序执行的位置。

addr2line就可以将40064b定位到代码的位置：

addr2line 40064b -e test -f -s -C

```
divide(int, int)
test.cpp:5
```

## 参考
- addr2line 
  https://www.jianshu.com/p/c2e2b8f8ea0d
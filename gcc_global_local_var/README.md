## 说明

## 注意

- 1.由于两个动态库中有相同的GLOBAL符号，先使用先出现的全局函数。
- 2.用LOCAL的符号只能在当前lib可见,全局不可见。

## 代码
- 1 全局函数默认全局可见，导致函数覆盖
- 2 全局函数默认全局可见，内部函数前面加上隐藏修饰符，对外不可见
- 3 全局函数默认不可见，对外函数前面加上可见修饰符，对外可见
- 4 全局函数默认不可见，全局变量对单独存在一份（类似于static）
- 5 全局函数默认不可见，对链接静态库同样有效 

## 参考
- GCC的符号可见性
https://blog.csdn.net/wakeupwakeup/article/details/50514841?utm_source=blogxgwz8
- nm显示符号类型
https://blog.csdn.net/farmwang/article/details/72778026
- 多个动态库存在同名函数处理方法
https://blog.csdn.net/qq_37887537/article/details/89472705
- 隐藏静态库中的符号
https://www.orcode.com/question/504769_kea106.html
- 通过文件配置GCC的静态库和动态库的符号导出
https://blog.csdn.net/dj0379/article/details/17011105
- 符号可见性简介
https://www.ibm.com/developerworks/cn/aix/library/au-aix-symbol-visibility/
## 说明
驱动开发简单示例

## 步骤
- 编译
make
- 安装
insmod memdev.ko
dmesg查看系统日志 
lsmod查看模块列表
- 卸载
rmmod memdev
- 安装节点
rm /dev/memdev0
mknod /dev/memdev0 c 243 0  

### 注意
- 主设备号240查看
通过注册的时候打印出来的 dmesg | tail
通过命令行查看 cat /proc/devices | grep memdev
- 指定主设备号会出错????

## 参考
1.字符驱动（非常不错的教程）
https://blog.csdn.net/Archar_Saber/article/details/85198183
2.ubuntu驱动开发
https://blog.csdn.net/mingtianwendy/article/details/77951666
3.创建dev节点
https://www.cnblogs.com/Ph-one/p/6720095.html?utm_source=itdadao&utm_medium=referral

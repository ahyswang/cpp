## 多个目录,多个源文件
需要分别在项目根目录 demo3 和 math 目录里各编写一个 CMakeLists.txt 文件。
为了方便，我们可以先将 math 目录里的文件编译成静态库再由 main 函数调用。
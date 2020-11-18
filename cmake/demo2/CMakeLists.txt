# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (demo2)
# 查找当前目录下的所有源文件
aux_source_directory(. SRCS)
# 指定生成目标
add_executable(demo2 ${SRCS})

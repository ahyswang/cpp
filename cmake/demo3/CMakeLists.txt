# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (demo3)
# 查找当前目录下的所有源文件
aux_source_directory(. SRCS)
# 添加include目录
include_directories(
	./math
)
# 添加math子目录
add_subdirectory(math)
# 指定生成目标
add_executable(demo3 main.cpp)
# 添加链接库
target_link_libraries(demo3 math)


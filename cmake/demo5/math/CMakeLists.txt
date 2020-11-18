# 查找当前目录下的所有源文件
aux_source_directory(. MATH_SRCS)
# 生成链接库
add_library(math ${MATH_SRCS})

# 指定 math 库的安装路径
install (TARGETS math DESTINATION bin)
install (FILES math_func.h DESTINATION include)

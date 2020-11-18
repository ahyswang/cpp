# 查找当前目录下的所有源文件
aux_source_directory(. MATH_SRCS)
# 生成链接库
add_library(math ${MATH_SRCS})
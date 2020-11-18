# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (demo5)

# 设置版本号
set (Demo_VERSION_MAJOR 1)
set (Demo_VERSION_MINOR 0)


# 检查系统是否支持 pow 函数
include (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
check_function_exists (pow HAVE_POW)

# 加入一个配置头文件，用于处理cmake对源码的设置
configure_file (
	"${PROJECT_SOURCE_DIR}/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
)

# 是否使用自己的math库
option (USE_MYMATH
		"Use provided math implementation" ON)

include_directories ("${PROJECT_BINARY_DIR}")
 
# 是否加入math库
if (USE_MYMATH)
	include_directories ("${PROJECT_SOURCE_DIR}/math")
	add_subdirectory(math)
	set (EXTRA_LIBS ${EXTRA_LIBS} math)
endif (USE_MYMATH)

# 查找当前目录下的所有源文件
aux_source_directory(. SRCS)

# 指定生成目标
add_executable(demo5 main.cpp)
# 添加链接库
target_link_libraries(demo5 ${EXTRA_LIBS})

# 设置安装路径
#set (CMAKE_INSTALL_PREFIX /tmp/demo5)
# 指定安装路径
install (TARGETS demo5 DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/config.h"
         DESTINATION include)

# 启用测试
enable_testing()
# 测试程序是否成功运行
# 测试 5 的平方
add_test (test_5_2 demo5 5 2)
set_tests_properties (test_5_2
 PROPERTIES PASS_REGULAR_EXPRESSION "is 25")

 # 定义一个宏，用来简化测试工作
 macro (do_test arg1 arg2 result)
	add_test(test_${arg1}_${arg2} demo5 ${arg1} ${arg2})
	set_tests_properties(test_${arg1}_${arg2}
		PROPERTIES PASS_REGULAR_EXPRESSION ${result})
 endmacro (do_test)

 do_test (5 2 "is 25")
 do_test (10 5 "is 100000")
 do_test (2 10 "is 1024")

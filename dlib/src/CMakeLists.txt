cmake_minimum_required(VERSION 2.8.12)

project(dlib)

set(CPACK_PACKAGE_NAME "dlib")
set(CPACK_PACKAGE_VERSION_MAJOR "19")
set(CPACK_PACKAGE_VERSION_MINOR "17")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})
# Only print these messages once, even if dlib is added multiple times via add_subdirectory()
if (NOT TARGET dlib)
   message(STATUS "Using CMake version: ${CMAKE_VERSION}")
   message(STATUS "Compiling dlib version: ${VERSION}")
endif()

# 指定编译文件
file(GLOB SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  ./sockets/*.cpp
#  ./msci_api/*.cpp
  ./*.cpp
)

# 指定生成目标
add_executable(demo2 ${SOURCES})
# 添加库

IF (WIN32)
	MESSAGE(STATUS "Now is windows")
ELSEIF (APPLE)
	MESSAGE(STATUS "Now is Apple systens.")
ELSEIF (UNIX)
	MESSAGE(STATUS "Now is UNIX-like OS's.")
	target_link_libraries(demo2 pthread)
ENDIF ()


PROJECT(apr)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")
MESSAGE(STATUS "operation host system name is ${CMAKE_HOST_SYSTEM_NAME}")
MESSAGE(STATUS "operation host system version is ${CMAKE_HOST_SYSTEM_VERSION}")
MESSAGE(STATUS "operation host system processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
	MESSAGE(STATUS "current platform: Linux")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
	MESSAGE(STATUS "current platform: Windows")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
	MESSAGE(STATUS "current platform: FreeBSD")
ELSE ()
	MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
 
SET(GPR_INCLUDE_DIRRECTORIES
	${PROECT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

#CONFIGURE_FILE(include/apr.hwc
#               ${PROJECT_BINARY_DIR}/apr.h)

INCLUDE_DIRECTORIES("${GPR_INCLUDE_DIRRECTORIES}")

SET(GPR_SOURCES
	src/apr_allocater.c
	src/apr_thread_win32.c
	src/apr_thread_linux.c
	src/apr_dso_win32.c
	src/apr_dso_linux.c
	src/apr_time_win32.c
	src/apr_time_linux.c
	src/apr_log_win32.c
	src/apr_log_linux.c
	src/apr_queue.c
	src/apr_rmm.c
	src/apr_version.c
	src/apr_strings.c
	src/apr_base64.c
	src/apr_strmatch.c
)

# Éú³ÉÁ´½Ó¿â
ADD_LIBRARY(gpr ${GPR_SOURCES})

SET(GPR_TEST_SOURCES 
	test/abts.c
	test/abts_tests.c
	test/testhello.c
	test/testthread.c
	test/teststring.c
	test/testqueue.c
	test/testrmm.c
	test/testtime.c
	test/testversion.c
	test/testlog.c
	test/testbase64.c
)

ADD_EXECUTABLE(testall ${GPR_TEST_SOURCES} ${GPR_SOURCES})

IF (WIN32)
SET(GPR_SYSTEM_LIBS
)
ELSEIF (UNIX)
SET(GPR_SYSTEM_LIBS  
	pthread
	dl
)
ENDIF ()


# Installation
INSTALL(TARGETS ${install_targets}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
       )

TARGET_LINK_LIBRARIES(testall ${GPR_SYSTEM_LIBS})
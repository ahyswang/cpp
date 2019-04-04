#mips
#SDK_PATH = /home/yswang/Project/mipsel-linux-android-4.9
#SDK_HOST = mipsel-linux-android
#CC = $(SDK_PATH)/bin/$(SDK_HOST)-gcc
#CXX = $(SDK_PATH)/bin/$(SDK_HOST)-g++
#LD =  $(SDK_PATH)/bin/$(SDK_HOST)-ld
#AR = $(SDK_PATH)/bin/$(SDK_HOST)-ar
#AS = $(SDK_PATH)/bin/$(SDK_HOST)-as
#STRIP = $(SDK_PATH)/bin/$(SDK_HOST)-strip

#arm7
#SDK_PATH = /home/yswang/Project/LS9_Toolchain/prebuilt/toolchain/armv7a
#SDK_HOST = armv7a-cros-linux-gnueabi
#CC = $(SDK_PATH)/bin/$(SDK_HOST)-gcc
#CXX = $(SDK_PATH)/bin/$(SDK_HOST)-g++
#LD =  $(SDK_PATH)/bin/$(SDK_HOST)-ld
#AR = $(SDK_PATH)/bin/$(SDK_HOST)-ar
#AS = $(SDK_PATH)/bin/$(SDK_HOST)-as
#STRIP = $(SDK_PATH)/bin/$(SDK_HOST)-strip

CCFLAGS += -g -Wall -O0 -fPIC
CFLAGS += -g -Wall -O0 -fPIC

TARGET_SHARE = libapr.so
TARGET_TEST = apr_test
TARGET_TEST_STATIC = apr_test_static

INCLUDES = -I./ -I./include/
LIBS = -L./ -lpthread -ldl

APR_SRCS = $(wildcard ./src/*.c)
APR_OBJS = $(APR_SRCS:.c=.o)
TEST_SRCS = $(wildcard ./test/*.c)
TEST_OBJS = $(TEST_SRCS:.c=.o)

all : $(TARGET_SHARE) $(TARGET_TEST) $(TARGET_TEST_STATIC)

$(TARGET_SHARE) : $(APR_OBJS)
	$(CC) -shared -fPIC -o $@ $^ $(LIBS)

$(TARGET_TEST) : $(TEST_OBJS)  
	$(CC) -o $@ $^ -L./ -lapr

$(TARGET_TEST_STATIC) : $(TEST_OBJS) $(APR_OBJS)
	$(CC) -o $@ $^ -L./ $(LIBS)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<  $(INCLUDES)

install:
	cp $(TARGET_SHARE) ../lib
	cp $(TARGET_TEST) ../bin
	cp $(TARGET_TEST_STATIC) ../bin

clean:
	-rm $(TARGET_SHARE) $(TARGET_TEST) $(APR_OBJS) $(TEST_OBJS) $(TARGET_TEST_STATIC)


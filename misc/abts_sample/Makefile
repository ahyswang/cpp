CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		abts.o testhello.o

LIBS =

TARGET =	abts.exe

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)
	
%.o: %.c
	$(CXX) -c $< $(CXXFLAGS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

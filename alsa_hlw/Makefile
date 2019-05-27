all: alsa_capture_hlw

INCLUDE = -I./ -I/home/yswang/temp/alsa_aarch64/include/
LIB = -L./ -L/home/yswang/temp/alsa_aarch64/lib/ -lasound

alsa_capture_hlw : alsa_capture_hlw.c
	$(CC) -o $@ $^ $(INCLUDE) $(LIB)


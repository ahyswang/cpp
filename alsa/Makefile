all : alsa_info alsa_play_pcm alsa_play_sin alsa_capture alsa_capture_hlw
	
alsa_info : alsa_info.c
	$(CC) $^ -o $@ -lasound -lm

alsa_play_pcm : alsa_play_pcm.c
	$(CC) $^ -o $@ -lasound -lm

alsa_play_sin : alsa_play_sin.c
	$(CC) $^ -o $@ -lasound -lm

alsa_capture : alsa_capture.c
	$(CC) $^ -o $@ -lasound -lm

alsa_capture_hlw : alsa_capture_hlw.c
	$(CC) $^ -o $@ -lasound -lm
	
tar :
	tar zcvf all.tar.gz alsa_info alsa_play_pcm alsa_play_sin play.pcm alsa_capture alsa_capture_hlw

clean :
	rm alsa_info alsa_play_pcm alsa_play_sin alsa_capture alsa_capture_hlw

	

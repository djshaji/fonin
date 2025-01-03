cc=$(CC) $(ARGS)
ARGS=-fPIC -g

all: tubeamp.o overdrive.so distortion.so sustain.so autowah.so
	$(CC) $(ARGS) -o fonin.so -fPIC -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -shared -lm *.o 
	
tubeamp.o: tubeamp.c tubeamp.h biquad.o
	$(CC) $(ARGS) -c tubeamp.c
	
overdrive.so: overdrive.c overdrive.h biquad.o
	$(CC) $(ARGS) -o overdrive.so -fPIC -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -shared -lm biquad.o overdrive.c

distortion.so: distortion.c distortion.h biquad.o rcfilter.o
	$(CC) $(ARGS) -o distortion.so -fPIC -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -shared -lm biquad.o rcfilter.o distortion.c
	
sustain.so: sustain.c sustain.h 
	$(CC) $(ARGS) -o sustain.so -fPIC -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -shared -lm sustain.c
	
autowah.so: autowah.c autowah.h backbuf.o utils.o audio-midi.c biquad.o
	$(CC) $(ARGS) -o autowah.so -fPIC -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -shared -lm biquad.o backbuf.o utils.o audio-midi.c autowah.c
	
biquad.o: biquad.c biquad.h utils.o
	$(CC) $(ARGS) -c biquad.c

utils.o: utils.c utils.h
	$(CC) $(ARGS) -c utils.c

backbuf.o: backbuf.c backbuf.h
	$(CC) $(ARGS) -c backbuf.c

clean:
	rm -v *.o

rcfilter.o: rcfilter.c rcfilter.h
	$(CC) $(ARGS) -c rcfilter.c

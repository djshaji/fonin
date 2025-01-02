cc=$(CC) $(ARGS)
ARGS=-fPIC -g

all: tubeamp.o
	$(CC) $(ARGS) -o fonin.so -fPIC -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -shared -lm *.o 
	
tubeamp.o: tubeamp.c tubeamp.h biquad.o
	$(CC) $(ARGS) -c tubeamp.c
	
overdrive.so: overdrive.c overdrive.h biquad.o
#	$(CC) $(ARGS) -c overdrive.c
	$(CC) $(ARGS) -o overdrive.so -fPIC -Wl,-Bstatic -Wl,-Bdynamic -Wl,--as-needed -shared -lm biquad.o overdrive.c
	
biquad.o: biquad.c biquad.h utils.o
	$(CC) $(ARGS) -c biquad.c

utils.o: utils.c utils.h
	$(CC) $(ARGS) -c utils.c

clean:
	rm -v *.o

CC = g++
CCFLAGS = -g -Wall

Main.bin: Main.cpp card.o 
		$(CC) -o Main.bin $(CCFLAGS) Main.cpp card.o -lm -lX11

# Add whatever additional files and rules here, and also
# # in the final linking rule above.
#
# rd_direct.o: rd_direct.cc rd_direct.h
# 	$(CC) $(CCFLAGS) -c rd_direct.cc
#
card.o: card.cpp card.h
# 		$(CC) $(CCFLAGS) -c pnm_display.cc
#
clean:
	-rm *.o Main.bin

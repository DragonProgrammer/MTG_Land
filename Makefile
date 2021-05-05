CC = clang++
CCFLAGS = -g -Wall

Main.bin: Main.cpp card.o actions.o mana.o
	$(CC) -o Main.bin $(CCFLAGS) Main.cpp card.o actions.o mana.o  -lm -lX11

# Add whatever additional files and rules here, and also
# # in the final linking rule above.
#
# rd_direct.o: rd_direct.cc rd_direct.h
# 	$(CC) $(CCFLAGS) -c rd_direct.cc
#
mana.o: mana.cpp mana.h
	$(CC) $(CCFLAGS) -c mana.cpp

actions.o: actions.cpp actions.h
	$(CC) $(CCFLAGS) -c actions.cpp
#
card.o: card.cpp card.h
	$(CC) $(CCFLAGS) -c card.cpp
#
clean:
	-rm *.o Main.bin

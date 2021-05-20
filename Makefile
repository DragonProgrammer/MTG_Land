CXX = clang++
CXXFLAGS = -g -Wall

Main.bin: Main.cpp card.o actions.o mana.o
	$(CXX) -o Main.bin $(CXXFLAGS) Main.cpp card.o actions.o mana.o  -lm -lX11

# Add whatever additional files and rules here, and also
# # in the final linking rule above.
#
# rd_direct.o: rd_direct.cc rd_direct.h
# 	$(CXX) $(CXXFLAGS) -c rd_direct.cc
#
mana.o: mana.cpp mana.h
	$(CXX) $(CXXFLAGS) -c mana.cpp

actions.o: actions.cpp actions.h
	$(CXX) $(CXXFLAGS) -c actions.cpp
#
card.o: card.cpp card.h
	$(CXX) $(CXXFLAGS) -c card.cpp
#
clean:
	-rm *.o Main.bin

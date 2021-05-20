CXX = clang++
WARNINGS    = -Wall -Werror
DEBUG_FLAGS = -g
CXXFLAGS = $(WARNINGS) $(DEBUG_FLAGS) 

HEADERS = $(wildcard *.h)
HPPEADERS = $(wildcard *.hpp)
OBJECTS = $(HEADERS:%.h=%.o) $(HPPEADERS:%.h=%.o)

Main.bin: Main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

##
# Code to check for `#include' statements.
##
DEPDIR := .dep
$(DEPDIR): ; mkdir $@
.PRECIOUS: $(DEPDIR)/%.d
$(DEPDIR)/%.d: %.cpp | $(DEPDIR)
	@set -e; rm -f $@; \
	$(CC) -MM $(CXXFLAGS) $< -MF $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
$(DEPDIR)/%.d: %.c | $(DEPDIR)
	@set -e; rm -f $@; \
	$(CC) -MM $(CXXFLAGS) $< -MF $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
%.o: .dep/%.d
DEPFILES := $(OBJECTS:%.o=$(DEPDIR)/%.d) $(DEPDIR)/Main.d
include $(DEPFILES)

clean:
	-rm *.o Main.bin

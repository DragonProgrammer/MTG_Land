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

.PHONY:tests
all:tests

##
# Find TODO tags and display them at the top of the output for convenience
##
.PHONY: TODO
tests: TODO
COLOR = always
# TODO: Better sort on line numbers.
TODO:
	@if grep --color=$(COLOR) -n "\(TODO\|FIXME|MAINTENANCE\)" $(HEADERS) $(CODE) > /dev/null; then \
	echo "+---------------------------- - - -  -   --    ---"; \
	echo "| List of TODO in code:"; \
	echo "|"; \
	grep --color=$(COLOR) -n "\(TODO\|FIXME\)" $(HEADERS) $(CODE) |sort |sed 's,\(.\),| - \1,' | sed 's,\s*//\s*,\t,' || true; \
	grep --color=$(COLOR) -n "MAINTENANCE" $(HEADERS) $(CODE) |sort |sed 's,\(.\),| - \1,' | sed 's,\s*//\s*,\t,' || true; \
	echo "+---------------------------- - - -  -   --    ---"; \
	echo ""; \
	fi

.PHONY: include_graph
tests: include_graph
include_graph:
	@if which graph-easy > /dev/null 2>&1; then \
		./.tools/cinclude2dot.pl 2>&1 | \
		grep -v "^Include file not found: <[^ ]*>" | \
		graph-easy; \
	else \
		echo "Please install graph library by running:"; \
		echo "$ cpan Graph::Easy"; \
	fi

##
# C and C++ Tools
##

##
# Lib Args
##
CCODE_FILES   ?= $(wildcard *.c)
CXXCODE_FILES ?= $(wildcard *.cpp)
CODE_FILES ?= $(CCODE_FILES) $(CPPCODE_FILES)
DEPDIR ?= .dep

##
# Code
##
CCPP_FILE_LOC := $(abspath $(lastword $(MAKEFILE_LIST)))
CCPP_FILE_DIR := $(notdir $(patsubst %/,%,$(dir $(CCPP_FILE_LOC))))

.PHONY: include_graph
# TODO: Just call `cpan`?
include_graph:
	@if which graph-easy > /dev/null 2>&1; then \
		$(CCPP_FILE_DIR)/ccxx/cinclude2dot.pl 2>&1 | \
		grep -v "^Include file not found: <[^ ]*>" | \
		graph-easy --boxart; \
	else \
		echo "Please install ASCII graph library by running:"; \
		echo "$ cpan Graph::Easy"; \
		echo "and installing GraphViz"; \
	fi

$(DEPDIR): ; @mkdir $@
.PRECIOUS: $(DEPDIR)/%.d
$(DEPDIR)/%.d: %.cpp | $(DEPDIR)
	@set -e; rm -f $@; \
	$(CXX) -MM $(CXXFLAGS) $< -MF $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
$(DEPDIR)/%.d: %.c | $(DEPDIR)
	@set -e; rm -f $@; \
	$(CC) -MM $(CXXFLAGS) $< -MF $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
%.o: .dep/%.d
.PHONY: CCPP_DEPS_CLEAN
clean: CCPP_DEPS_CLEAN
CCPP_DEPS_CLEAN: ; @-rm -rf "$(DEPDIR)"


# TODO: Check for doxygen
#Doxyfile:
#	doxygen -g > /dev/null

ProjectFlags = -fPIE -I ./json/include -pthread -fexceptions
SANS=

##
# Set up the build environment and settings
# ##
SHELL = /bin/bash
#COLOR = $(shell echo $$TERM|grep color > /dev/null && echo always || echo auto)
COLOR = always
CXX = g++
LanguageVersion = -std=c++2a -fno-exceptions
Should_be_Default_Warnings = -Wuninitialized -Wshadow
Real_Warnings = -Wall -Wextra -Wmissing-field-initializers -Wformat=2
Smell_Warnings = -Wunused -Wno-padded
NoWarn = -Wno-dangling-else -Wno-c++98-compat -Wno-old-style-cast -Wno-unused-macros -Wno-comma 
#SANS=address
ifeq ($(SANS),)
 SANITIZER=
else
 SANITIZER= -g -fsanitize=$(SANS) -ftrapv
endif

Warnings = $(Should_be_Default_Warnings) $(Real_Warnings)
Debugging = -Werror -Wfatal-errors -fdiagnostics-color=$(COLOR) $(SANITIZER) $(Smell_Warnings)
CXXFLAGS = $(LanguageVersion) $(Warnings) $(NoWarn) $(Debugging) $(ProjectFlags)

HEADER_FILES  = $(wildcard *.h) $(wildcard *.hpp)
CCODE_FILES   = $(wildcard *.c)
CXXCODE_FILES = $(wildcard *.cpp)
CODE_FILES    = $(CCODE_FILES) $(CXXCODE_FILES)
SOURCE_FILES  = $(HEADER_FILES) $(CODE_FILES)

##
# List of library flags for linker
##
LIBS          = 
LDFLAGS       = $(shell pkg-config --libs $(LIBS)) -ltinfo 

all: card_parse
json_test: card_parse.o
	$(CXX) $(CXXFLAGS) -o $@ $^

##
# Pseudo-target to remove the files made by this Makefile
##
.PHONY: clean

# Is there a better way to make this load as the always first target?
# Using =include= makes an infinite recursion.
all: .make_update
watch_target: .make_update
.make_update: Makefile
	$(MAKE) clean
	 	touch $@

##
# Describe the actual program structure.
# These This is the only important line for building the program.
##
LINK.o = $(LINK.cc)
include $(wildcard *.make)


.PHONY: clean_temps
clean: clean_temps
clean_temps:
	-rm *.tmp
	-rm .*.tmp

.PHONY: clean_objects
clean: clean_objects
clean_objects:
	-rm *.o

.PHONY: clean_targets
clean: clean_targets
clean_targets:
	-rm edgit
	-rm *.dot


##
# Include and generate dependency files (ccxx.make)
##
DEPDIR   := .dep
DEPFILES := $(CCODE_FILES:%.c=$(DEPDIR)/%.d) $(CXXCODE_FILES:%.cpp=$(DEPDIR)/%.d)
include $(DEPFILES)

%.d: ; mkdir $@

##
# Make a pseudo-target for the tests
##
# Also, make a happy message at the end if they all passed.
.PHONY: %.test
.test.d/%: % | .test.d
	./$<
	touch $@

.PHONY: tests
tests:
	@$(MAKE) Message_Box MESSAGE="Tests Compleate" --no-print-directory
watch_target: tests

.PHONY: clean_tests
clean: clean_tests
clean_tests:
	-rm .test.d -r
	-rm *_test

# Built the real target
watch_all: all
	@$(MAKE) Message_Box MESSAGE="Built ALL Target" --no-print-directory
watch_target: watch_all
watch_target: show_TODOs

##
# After the tests pass, give a list of missing documentation.
##
.PHONY: docs
watch_target: docs
docs: docs_message

.PHONY: docs_message
docs_message:
	@$(MAKE) Message_Box MESSAGE="Checking for Missing Documentation" --no-print-directory
docs: .doc_time
	.doc_time: $(HEADER_FILES) Doxyfile Makefile
	@doxygen 2>&1 | grep "warning" | sed 's,.*/,,' | tee $@.tmp | head -n 30
	@diff $@.tmp /dev/null > /dev/null
	@mv $@.tmp $@
Doxyfile: Makefile
	@echo -n "INPUT = " > Doxyfile
	@echo "$(HEADER_FILES)" >> Doxyfile

.PHONY: clean_docs
clean: clean_docs
clean_docs:
	-rm -f Doxyfile .doc_time
	-rm -rf html latex

##
# If the docs are done, draw a graph of the includes.
##
watch_target: include_graph_message include_graph
include_graph_message:
	@$(MAKE) Message_Box MESSAGE="Rendering \"include\"s Graph" --no-print-directory

##
# Very small "interactive debugger"
##
.PHONY: watch_target
watch_target:
	@$(MAKE) Message_Box MESSAGE="All tests passed and All documentation is done." --no-print-directory
.PHONY: watch
watch:
#watch --color "$(MAKE) _watch_wait && timeout 10 $(MAKE) watch_target"
	watch --color "timeout 10 $(MAKE) watch_target"
.PHONY: _watch_wait

# This is a separate directive so that the lists update.
IMPORTANT_FILES = edgit.org
_watch_wait:
	@inotifywait $(IMPORTANT_FILES)

##
# Include my make libraries
##
include ./make_tools-master/.make_tools-master/ccxx.make
include ./make_tools-master/.make_tools-master/lint.make
include ./make_tools-master/.make_tools-master/message.make

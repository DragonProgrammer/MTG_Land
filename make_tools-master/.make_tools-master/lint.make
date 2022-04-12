##
# Check for basic code maintenance
##

##
# Args
##
SOURCE_FILES ?= $(wildcard *.cpp) $(wildcard *.hpp)

##
# Code
##

##
# Find TODO tags and display them at the top of the output for convenience
##
# TODO: Rename this, maybe "MKlint_show_todos" or something similar to
# namespace the function.
.PHONY: show_TODOs
# TODO: Better sort on line numbers.
show_TODOs:
	@if grep --color=$(COLOR) -n "\(TODO\|FIXME|MAINTENANCE\)" $(SOURCE_FILES) > /dev/null; then \
	$(MAKE) Message_Section --no-print-directory; \
	echo "| List of TODO in code:"; \
	echo "|"; \
	grep --color=$(COLOR) -n "\(TODO\|FIXME\)" $(SOURCE_FILES) |sort |sed 's,\(.\),| - \1,' | sed 's,\s*//\s*,\t,' || true; \
	grep --color=$(COLOR) -n "MAINTENANCE" $(SOURCE_FILES) |sort |sed 's,\(.\),| - \1,' | sed 's,\s*//\s*,\t,' || true; \
	$(MAKE) Message_Section --no-print-directory; \
	echo ""; \
	fi

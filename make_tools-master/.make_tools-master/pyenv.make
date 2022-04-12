# -*- mode: GNUmakefile -*-

PY3      ?= python3
PIP3     ?= $(PY3) -m pip
VIRT_ENV ?= .virtualenv

.PRECIOUS: $(VIRT_ENV)
$(VIRT_ENV):
	@printf "%-71s ... " "Generating Python ($(PY3)) Virtual Env"
	#TODO `ensurepip`?
	@$(PIP3) install --user virtualenv -q
	@$(PY3) -m virtualenv "$@" -q
	@echo "Done"

VIRT_PY  ?=$(VIRT_ENV)/bin/$(PY3)
VIRT_PIP ?=$(VIRT_ENV)/bin/pip3

.PRECIOUS: $(VIRT_PY) $(VIRT_PIP)
$(VIRT_PY) $(VIRT_PIP): $(VIRT_ENV) ;

.PRECIOUS: $(VIRT_ENV)/lib/$(PY3)/site-packages/%
$(VIRT_ENV)/lib/$(PY3)/site-packages/%: $(VIRT_PIP)
	@printf "%-71s ... " "Installing ($(PY3)) $*"
	@$(VIRT_PIP) install "$*" -q
	@echo "Done"
.PHONY: %.pylib
%.pylib: $(VIRT_ENV)/lib/$(PY3)/site-packages/% ;

.PHONY: clean_env
clean_all: clean_env
clean_env:
	-rm -rf $(VIRT_ENV)

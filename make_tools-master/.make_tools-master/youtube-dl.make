#!/usr/bin/env -S make -f

VDL_PROG        ?= youtube-dl
VDL_NAME_FORMAT ?= "%(uploader)s - %(upload_date)s - %(id)s - %(title).150s.%(ext)s"
HISTORY_ARCHIVE ?= $(shell pwd)/.dl_archive
VDL_CONFIG      ?= --ignore-config --write-description --restrict-filenames --all-subs --retries 10 --fragment-retries 10 --ignore-errors
VDL_ARCHIVE     ?= --download-archive $(HISTORY_ARCHIVE)
VDL_FLAGS       ?= $(VDL_CONFIG) $(VDL_ARCHIVE) -o $(VDL_NAME_FORMAT)
VDL             ?= $(VLD_PROG) $(VDL_FLAGS) -a

.PHONY: %.yt-dl
%.yt-dl: %.subscription %.d
	cd "$*.d" ; $(VDL) "../$<"
# TODO suppress or intent messages?

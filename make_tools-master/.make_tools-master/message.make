
# TODO: Multiline handling.
Message_Box:
	@printf "_+%*s+\n" "$$(expr $$(tput cols) - 4)" '' | tr ' _' '- '
	@printf " | %-$$(expr $$(tput cols) - 6)s |\n" "$(MESSAGE)"
	@printf "_+%*s+\n" "$$(expr $$(tput cols) - 4)" '' | tr ' _' '- '

# TODO: Accept an optional section title.
# TODO: Accept a second make command to call and wrap with left '|' marks.
Message_Section:
	@echo "+---------------------------- - - -  -   --    ---"

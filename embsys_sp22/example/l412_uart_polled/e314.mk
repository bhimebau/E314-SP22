# E314 Local targets

etags:                                                                          
	find . -type f -iname "*.[ch]" | xargs etags --append

gdb: $(BUILD_DIR)/$(TARGET).elf
	cp gdbinit ./build/.gdbinit
	openocd -f board/st_nucleo_l4.cfg 



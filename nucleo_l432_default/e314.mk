# E314 Local targets

download: $(BUILD_DIR)/$(TARGET).bin                                            
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000 > st-flash.log 2>&1

etags:                                                                          
	find . -type f -iname "*.[ch]" | xargs etags --append

gdb:
	cp gdbinit ./build/.gdbinit
	openocd -f board/st_nucleo_l4.cfg 

reset:
	st-flash reset


# E314 Local targets

etags:                                                                          
	find . -type f -iname "*.[ch]" | xargs etags --append

gdb:
	cp gdbinit ./build/.gdbinit
	../../openocd/bin/openocd -s ../../openocd/share/scripts -f board/st_nucleo_l4.cfg 
reset:
	st-flash reset


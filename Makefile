

ifeq ($(config), release)
	OPTS:=-std=gnu99 -g -O3 --pedantic-errors
else
	OPTS:=-fsanitize=address -fsanitize=undefined -std=gnu99 -g -O0 --pedantic-errors -Wall -Wextra -Wstrict-prototypes
endif

CFLAGS:=-lm

ALL: nuklear_fb terminal_fb

nuklear_fb: nuklear_filebrowser.c filebrowser.c file.c filebrowser.h
	$(CC) $(OPTS) nuklear_filebrowser.c filebrowser.c file.c -o nuklear_fb $(CFLAGS) `pkg-config sdl2 --cflags --libs`


terminal_fb: terminal_filebrowser.c c_utils.c filebrowser.c file.c
	$(CC) $(OPTS) terminal_filebrowser.c c_utils.c filebrowser.c file.c -o terminal_fb $(CFLAGS)


clean:
	rm *.o terminal_fb nuklear_fb

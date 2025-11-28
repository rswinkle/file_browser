

ifeq ($(config), release)
	OPTS:=-std=gnu99 -g -O3 --pedantic-errors
else
	OPTS:=-fsanitize=address -fsanitize=undefined -std=gnu99 -g -O0 --pedantic-errors -Wall -Wextra -Wstrict-prototypes -Wunused
endif

CFLAGS:=-Isrc -lm

ALL: nuklear_fb terminal_fb

nuklear_fb: nuklear_filebrowser.c file_browser.h
	$(CC) $(OPTS) nuklear_filebrowser.c -o $@ $(CFLAGS) `pkg-config sdl2 --cflags --libs`
terminal_fb: terminal_filebrowser.c file_browser.h
	$(CC) $(OPTS) terminal_filebrowser.c src/c_utils.c -o $@ $(CFLAGS)

clean:
	rm *.o terminal_fb nuklear_fb

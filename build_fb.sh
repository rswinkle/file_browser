OPTS="-fsanitize=address -fsanitize=undefined -std=gnu99 -g -O0 -Wall"
echo ${OPTS}

${CC:=gcc} -std=gnu99 -g -O0 terminal_filebrowser.c c_utils.c filebrowser.c file.c -o terminal_fb_test -I../src
${CC:=gcc} ${OPTS} nuklear_filebrowser.c filebrowser.c file.c -o nuklear_fb_test -I../src `pkg-config sdl2 --cflags --libs` -lm

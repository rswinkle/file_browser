
#define FILE_LIST_SZ 20
#define FILE_TYPE_STR "Images"
#define FILE_BROWSER_IMPLEMENTATION
#include "file_browser.h"

#include "myinttypes.h"
#include "c_utils.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>


#define NUM_DFLT_EXTS 11

cvector_file files;

int print_browser(file_browser* fb);


int main(int argc, char** argv)
{
	const char* default_exts[NUM_DFLT_EXTS] =
	{
		".jpg",
		".jpeg",
		".gif",
		".png",
		".bmp",

		".ppm",
		".pgm",

		".tga",
		".hdr",
		".pic",
		".psd"
	};

	char* start_dir = NULL;
	if (argc == 2) {
		start_dir = argv[1];
	}

	file_browser browser = { 0 };

	init_file_browser(&browser, default_exts, NUM_DFLT_EXTS, start_dir, NULL, NULL);

	while (!browser.file[0]) {
		if (!print_browser(&browser)) break;
	}

	if (browser.file[0]) {
		printf("Selected: %s\n", browser.file);
	} else {
		printf("No file selected\n");
	}

	free_file_browser(&browser);


	return 0;
}


int print_browser(file_browser* fb)
{
	cvector_file* f = &fb->files;

	printf("H. home = %s\n", fb->home);
	printf("D. desktop = %s\n", fb->desktop);
	printf("P. To parent directory\n");

	printf("dir = %s\n", fb->dir);
	
	printf("There are %"PRIcv_sz" files\n", f->size);
	puts("N. Sort by name");
	puts("Z. Sort by size");
	puts("M. Sort by last modified");
	puts("Q. Quit/Exit\n");

	int invalid = 0;

	for (int i=fb->begin, j=0; i<fb->end; i++, j++) {
		printf("%2d %-40s%20s%30s\n", j, f->a[i].name, f->a[i].size_str, f->a[i].mod_str);
	}

	printf("\nS. down\nW. up\nF. choose file\n");
	printf("Enter selection: ");

	char line_buf[STRBUF_SZ];

	char choice = read_char(stdin, SPACE_SET, 0, 1);
	if (choice == 'q' || choice == 'Q') {
		return 0;
	}

	if (choice == 'f' || choice == 'F') {
		int fn = 0;
		do {
			printf("Enter file number: ");
			
			// TODO properly
			freadstring_into_str(stdin, '\n', line_buf, STRBUF_SZ);
		} while (sscanf(line_buf, "%d", &fn) != 1 || fn < 0 || fn >= fb->end - fb->begin);

		int idx = fb->begin + fn;
		printf("fn = %d, idx = %d\n", fn, idx);


		// it's a directory, switch to it
		if (f->a[idx].size == -1) {
			switch_dir(fb, f->a[idx].path);
		} else {
			strncpy(fb->file, f->a[idx].path, MAX_PATH_LEN);
		}
	} else {
		char* c;
		invalid = 0;
		switch (choice) {
			case 'H':
			case 'h':
				switch_dir(fb, fb->home);
				break;
			case 'D':
			case 'd':
				switch_dir(fb, fb->desktop);
				break;
			case 'P':
			case 'p':
				c = strrchr(fb->dir, '/');
				if (c != fb->dir) {
					*c = 0;
					switch_dir(fb, NULL);
				} else {
					switch_dir(fb, "/");
				}
				break;
			case 'S':
			case 's':
				fb->begin += FILE_LIST_SZ;
				if (fb->begin >= fb->files.size) {
					fb->begin = 0;
				}
				break;
			case 'W':
			case 'w':
				// TODO should I wrap around the top like I do the bottom?
				fb->begin -= FILE_LIST_SZ;
				if (fb->begin < 0) {
					// wrap if you were already at the top
					if (fb->begin == -FILE_LIST_SZ && fb->files.size > FILE_LIST_SZ) {
						fb->begin = fb->files.size - FILE_LIST_SZ;
					} else {
						fb->begin = 0;  // just go all the way to the top
					}
				}
				break;
			case 'N':
			case 'n':
				if (fb->sorted_state == NAME_UP) {
					qsort(fb->files.a, fb->files.size, sizeof(file), filename_cmp_gt);
					fb->sorted_state = NAME_DOWN;
				} else {
					qsort(fb->files.a, fb->files.size, sizeof(file), filename_cmp_lt);
					fb->sorted_state = NAME_UP;
				}
				break;
			case 'Z':
			case 'z':
				if (fb->sorted_state == SIZE_UP) {
					qsort(fb->files.a, fb->files.size, sizeof(file), filesize_cmp_gt);
					fb->sorted_state = SIZE_DOWN;
				} else {
					qsort(fb->files.a, fb->files.size, sizeof(file), filesize_cmp_lt);
					fb->sorted_state = SIZE_UP;
				}
				break;
			case 'M':
			case 'm':
				if (fb->sorted_state == MODIFIED_UP) {
					qsort(fb->files.a, fb->files.size, sizeof(file), filemodified_cmp_gt);
					fb->sorted_state = MODIFIED_DOWN;
				} else {
					qsort(fb->files.a, fb->files.size, sizeof(file), filemodified_cmp_lt);
					fb->sorted_state = MODIFIED_UP;
				}
				break;

			default:
				invalid = 1;
				puts("Invalid choice.");
		}
	}

	if (!invalid) {
		fb->end = fb->begin + FILE_LIST_SZ;
		if (fb->end > fb->files.size) {
			fb->end = fb->files.size;
		}
	}

	return 1;
}









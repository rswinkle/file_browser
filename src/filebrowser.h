#ifndef FILEBROWSER_H
#define FILEBROWSER_H


// pulls in cvector
#include "file.h"

#ifndef FILE_TYPE_STR
#define FILE_TYPE_STR "Match Exts"
#endif

#define TRUE 1
#define FALSE 0

#ifndef FB_LOG
#define FB_LOG(A, ...) printf(A, __VA_ARGS__)
#endif

#define STRBUF_SZ 512
#define MAX_PATH_LEN STRBUF_SZ
#define PATH_SEPARATOR '/'

typedef int (*recents_func)(cvector_str* recents, void * userdata);
typedef int (*cmp_func)(const void* a, const void* b);
enum { FB_NAME_UP, FB_NAME_DOWN, FB_SIZE_UP, FB_SIZE_DOWN, FB_MODIFIED_UP, FB_MODIFIED_DOWN };
enum { FB_NAME, FB_SIZE, FB_MODIFIED };

// TODO name? file_explorer? selector?
typedef struct file_browser
{
	char dir[MAX_PATH_LEN];   // cur location
	char file[MAX_PATH_LEN];  // return "value" ie file selected 

	// special bookmarked locations
	char home[MAX_PATH_LEN];
	char desktop[MAX_PATH_LEN];

	// searching
	char text_buf[STRBUF_SZ];
	int text_len;

	recents_func get_recents;
	void* userdata;

	// bools
	int is_recents;
	int is_search_results;
	int is_text_path; // could change to flag if I add a third option
	int list_setscroll;
	int show_hidden;
	int select_dir;

	// does not own memory
	const char** exts;
	int num_exts;
	int ignore_exts; // if true, show all files, not just matching exts

	// list of files in cur directory
	cvector_file files;

	cvector_i search_results;
	int selection;

	// Not used internally, only if the user wants to control the
	// list view see terminal_filebrowser.c
#ifdef FILE_LIST_SZ
	int begin;
	int end;
#endif

	int sorted_state;
	cmp_func c_func;

} file_browser;

int init_file_browser(file_browser* browser, const char** exts, int num_exts, const char* start_dir, recents_func r_func, void* userdata);
void free_file_browser(file_browser* fb);
void switch_dir(file_browser* fb, const char* dir);
void handle_recents(file_browser* fb);

void fb_search_filenames(file_browser* fb);

// TODO think about this
void fb_sort_toggle(file_browser* fb, int sort_type);
#define fb_sort_name(fb) fb_sort_toggle((fb), FB_NAME)
#define fb_sort_size(fb) fb_sort_toggle((fb), FB_SIZE)
#define fb_sort_modified(fb) fb_sort_toggle((fb), FB_MODIFIED)

const char* get_homedir(void);
int fb_scandir(cvector_file* files, const char* dirpath, const char** exts, int num_exts, int show_hidden, int select_dir);
char* mydirname(const char* path, char* dirpath);
char* mybasename(const char* path, char* base);
void normalize_path(char* path);
int bytes2str(int bytes, char* buf, int len);


#endif


#define CLNK_IMPLEMENTATION
#include "clnk.h"

#define FILE_TYPE_STR "Images"
#define FILE_BROWSER_IMPLEMENTATION
#include "file_browser.h"

//#include "myinttypes.h"


#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>


#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sdl_renderer.h"



#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define STRBUF_SZ 512

#define FONT_SIZE 16
#define NUM_DFLT_EXTS 11

#define UNUSED(X) (void)X

enum { SORT_NAME, SORT_PATH, SORT_SIZE, SORT_MODIFIED, NUM_USEREVENTS };

// Better names/macros
enum {
	NORMAL           = 0x1,
	THUMB_DFLT       = 0x2,
	THUMB_VISUAL     = 0x4,
	THUMB_SEARCH     = 0x8,
	LIST_DFLT        = 0x10,
	SEARCH_RESULTS   = 0x20,
	VIEW_RESULTS     = 0x40,
	VIEWED_RESULTS   = 0x80
};



typedef struct global_state
{
	SDL_Window* win;
	SDL_Renderer* ren;
	struct nk_context *ctx;

	int scr_w;
	int scr_h;

	// do these belong here or in file_browser?
	int selection;
	int list_setscroll;
	u32 userevent;

	struct nk_color bg;

	float x_scale;
	float y_scale;

	cvector_str bookmarks;

} global_state;

global_state g_state;
global_state* g = &g_state;

int do_filebrowser(file_browser* fb, struct nk_context* ctx, int scr_w, int scr_h);

int linux_recents(cvector_str* recents, void* userdata);
int windows_recents(cvector_str* recents, void* userdata);

int handle_events(file_browser* fb, struct nk_context* ctx);

int main(int argc, char** argv)
{
	/* SDL setup */
	if (SDL_Init(SDL_INIT_VIDEO) == -1) {
		printf( "Can't init SDL:  %s\n", SDL_GetError( ) );
		return 1;
	}

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

	int win_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;

	g->win = SDL_CreateWindow("File Selector",
	                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	                       WINDOW_WIDTH, WINDOW_HEIGHT,
	                       win_flags);

	if (!g->win) {
		printf("Can't create window: %s\n", SDL_GetError());
		return 1;
	}
	/* try VSYNC and ACCELERATED */
	g->ren = SDL_CreateRenderer(g->win, -1, SDL_RENDERER_SOFTWARE);
	if (!g->ren) {
		printf("Can't create ren: %s\n", SDL_GetError());
		return 1;
	}

	float hdpi, vdpi, ddpi;
	SDL_GetDisplayDPI(0, &ddpi, &hdpi, &vdpi);
	printf("DPIs: %.2f %.2f %.2f\n", ddpi, hdpi, vdpi);

	SDL_Rect r;
	SDL_GetDisplayBounds(0, &r);
	printf("display bounds: %d %d %d %d\n", r.x, r.y, r.w, r.h);

	g->x_scale = 1; //hdpi/72;  // adjust for dpi, then go from 8pt font to 12pt
	g->y_scale = 1; //vdpi/72;

	g->userevent = SDL_RegisterEvents(1);
	if (g->userevent == (u32)-1) {
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Error: %s\n", SDL_GetError());
		return 0;
	}

	/*
	if (SDL_RenderSetLogicalSize(g->ren, WINDOW_WIDTH*g->x_scale, WINDOW_HEIGHT*g->y_scale)) {
		printf("logical size failure: %s\n", SDL_GetError());
		return 1;
	}
	*/

	if (!(g->ctx = nk_sdl_init(g->win, g->ren))) {
		printf("nk_sdl_init() failed!");
		return 1;
	}

	// TODO Font stuff, refactor/reorganize
	int render_w, render_h;
	int window_w, window_h;
	SDL_GetRendererOutputSize(g->ren, &render_w, &render_h);
	SDL_GetWindowSize(g->win, &window_w, &window_h);
	g->x_scale = (float)(render_w) / (float)(window_w);
	g->y_scale = (float)(render_h) / (float)(window_h);
	// could adjust for dpi, then adjust for font size if necessary
	//g->x_scale = 2; //hdpi/72;
	//g->y_scale = 2; //vdpi/72;
	//SDL_RenderSetScale(g->ren, g->x_scale, g->y_scale);
	float font_scale = g->y_scale;

	printf("scale %f %f\n", g->x_scale, g->y_scale);
	SDL_RenderSetScale(g->ren, g->x_scale, g->y_scale);
	nk_sdl_scale(g->x_scale, g->y_scale);

	struct nk_font_atlas* atlas;
	struct nk_font_config config = nk_font_config(0);
	struct nk_font* font;

	nk_sdl_font_stash_begin(&atlas);
	font = nk_font_atlas_add_default(atlas, FONT_SIZE*font_scale, &config);
	//font = nk_font_atlas_add_from_file(atlas, "../fonts/kenvector_future_thin.ttf", 13 * font_scale, &config);
	nk_sdl_font_stash_end();

	font->handle.height /= font_scale;
	nk_style_set_font(g->ctx, &font->handle);

	struct nk_style_toggle* tog = &g->ctx->style.option;
	printf("padding = %f %f border = %f\n", tog->padding.x, tog->padding.y, tog->border);
	//tog->padding.x = 2;
	//tog->padding.y = 2;

    static struct nk_color color_table[NK_COLOR_COUNT];
    memcpy(color_table, nk_default_color_style, sizeof(color_table));

	if (SDL_GetDisplayUsableBounds(0, &r)) {
		SDL_Log("Error getting usable bounds: %s\n", SDL_GetError());
		r.w = WINDOW_WIDTH;
		r.h = WINDOW_HEIGHT;
	} else {
		SDL_Log("Usable Bounds: %d %d %d %d\n", r.x, r.y, r.w, r.h);
	}
    g->scr_w = WINDOW_WIDTH;
    g->scr_h = WINDOW_HEIGHT;

	file_browser browser = { 0 };

	char* start_dir = NULL;
	if (argc == 2) {
		start_dir = argv[1];
	}

	// TODO MacOS?
#ifndef _WIN32
	init_file_browser(&browser, default_exts, NUM_DFLT_EXTS, start_dir, linux_recents, NULL);
#else
	init_file_browser(&browser, default_exts, NUM_DFLT_EXTS, start_dir, windows_recents, NULL);
#endif

	// default no no selection
	browser.selection = -1;

	//struct nk_colorf bg2 = nk_rgb(28,48,62);
	g->bg = nk_rgb(0,0,0);
	while (1) {
		//SDL_RenderSetScale(ren, x_scale, y_scale);

		if (handle_events(&browser, g->ctx))
			break;

		/* GUI */
		if (!do_filebrowser(&browser, g->ctx, g->scr_w, g->scr_h)) {
			break;
		}

		SDL_SetRenderDrawColor(g->ren, g->bg.r, g->bg.g, g->bg.b, g->bg.a);
		SDL_RenderSetClipRect(g->ren, NULL);
		SDL_RenderClear(g->ren);

		SDL_RenderSetScale(g->ren, g->x_scale, g->y_scale);
		nk_sdl_render(NK_ANTI_ALIASING_ON);
		SDL_RenderSetScale(g->ren, 1, 1);

		SDL_RenderPresent(g->ren);
		SDL_Delay(15);
	}

	if (browser.file[0]) {
		printf("Selected: %s\n", browser.file);
	} else {
		printf("No file selected\n");
	}

	free_file_browser(&browser);

	nk_sdl_shutdown();
	SDL_DestroyRenderer(g->ren);
	SDL_DestroyWindow(g->win);
	SDL_Quit();
	return 0;
}


int handle_events(file_browser* fb, struct nk_context* ctx)
{
	SDL_Event e;
	int sym;
	int code, sort_timer;
	int ret = 0;
	int did_sort = 0;
	//SDL_Keymod mod_state = SDL_GetModState();
	
	cvector_file* f = &fb->files;

	nk_input_begin(ctx);
	while (SDL_PollEvent(&e)) {
		// TODO edit menu/GUI as appropriate for list mode, see which
		// actions make sense or are worth supporting (re-evaluate if I
		// have some sort of preview)
		if (e.type == g->userevent) {
			code = e.user.code;
			switch (code) {
			case SORT_NAME:
				SDL_Log("Starting sort by name\n");
				sort_timer = SDL_GetTicks();
				if (fb->sorted_state != NAME_UP) {
					qsort(f->a, f->size, sizeof(file), filename_cmp_lt);
					fb->sorted_state = NAME_UP;
					fb->c_func = filename_cmp_lt;
				} else {
					qsort(f->a, f->size, sizeof(file), filename_cmp_gt);
					fb->sorted_state = NAME_DOWN;
					fb->c_func = filename_cmp_gt;
				}
				did_sort = TRUE;
				SDL_Log("Sort took %d\n", SDL_GetTicks()-sort_timer);
				break;
			case SORT_SIZE:
				SDL_Log("Starting sort by size\n");
				sort_timer = SDL_GetTicks();
				if (fb->sorted_state != SIZE_UP) {
					qsort(f->a, f->size, sizeof(file), filesize_cmp_lt);
					fb->sorted_state = SIZE_UP;
					fb->c_func = filesize_cmp_lt;
				} else {
					qsort(f->a, f->size, sizeof(file), filesize_cmp_gt);
					fb->sorted_state = SIZE_DOWN;
					fb->c_func = filesize_cmp_gt;
				}
				did_sort = TRUE;
				SDL_Log("Sort took %d\n", SDL_GetTicks()-sort_timer);
				break;
			case SORT_MODIFIED:
				SDL_Log("Starting sort by modified\n");
				sort_timer = SDL_GetTicks();
				if (fb->sorted_state != MODIFIED_UP) {
					qsort(f->a, f->size, sizeof(file), filemodified_cmp_lt);
					fb->sorted_state = MODIFIED_UP;
					fb->c_func = filemodified_cmp_lt;
				} else {
					qsort(f->a, f->size, sizeof(file), filemodified_cmp_gt);
					fb->sorted_state = MODIFIED_DOWN;
					fb->c_func = filemodified_cmp_gt;
				}
				did_sort = TRUE;
				SDL_Log("Sort took %d\n", SDL_GetTicks()-sort_timer);
				break;
			default:
				SDL_Log("Unknown user event!");
			}
			if (did_sort && fb->is_search_results) {
				fb_search_filenames(fb);
			}
			continue;
		}
		switch (e.type) {
		case SDL_QUIT:
			// don't think I really need these since we'll be exiting anyway
			nk_input_end(ctx);
			return 1;
		case SDL_KEYUP:
			sym = e.key.keysym.sym;
			switch (sym) {
			case SDLK_ESCAPE:
				if (fb->is_search_results) {
					fb->text_buf[0] = 0;
					fb->text_len = 0;
					fb->is_search_results = FALSE;
					if (fb->selection >= 0) {
						fb->selection = fb->search_results.a[fb->selection];
					}
					fb->list_setscroll = TRUE;
				} else {
					nk_input_end(ctx);
					return 1;
				}
				break;

			// switch to normal mode on that image
			case SDLK_RETURN:
				if (fb->selection >= 0) {
					int sel = (fb->is_search_results) ? fb->search_results.a[fb->selection] : fb->selection;
					if (f->a[sel].size == -1) {
						switch_dir(fb, f->a[sel].path);
					} else {
						strncpy(fb->file, f->a[sel].path, MAX_PATH_LEN);
						ret = 1;
					}
				}
				break;
			}
			break;

		case SDL_KEYDOWN:
			sym = e.key.keysym.sym;
			switch (sym) {
			case SDLK_UP:
			case SDLK_DOWN:
				if (!fb->text_len || fb->is_search_results) {
					fb->selection += (sym == SDLK_DOWN || sym == SDLK_j) ? 1 : -1;
					if (fb->is_search_results) {
						if (fb->selection < 0)
							fb->selection += fb->search_results.size;
						else
							fb->selection %= fb->search_results.size;
					} else {
						if (fb->selection < 0)
							fb->selection += f->size;
						else
							fb->selection %= f->size;
					}
					fb->list_setscroll = TRUE;
				}
				break;
			}
			break; //KEYDOWN

		case SDL_WINDOWEVENT: {
			//g->status = REDRAW;
			int x, y;
			SDL_GetWindowSize(g->win, &x, &y);
			//SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "windowed %d %d %d %d\n", g->scr_w, g->scr_h, x, y);
			switch (e.window.event) {
			case SDL_WINDOWEVENT_RESIZED:
				g->scr_w = e.window.data1;
				g->scr_h = e.window.data2;
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				g->scr_w = e.window.data1;
				g->scr_h = e.window.data2;

				break;
			}
		} break;

		//case SDL_MOUSEBUTTONUP:
			//printf("mouse click: %d, %d\n", e.button.x, e.button.y);

		break;

		// all other event types
		default:
			break;
		}

		nk_sdl_handle_event(&e);
	}
	nk_input_end(g->ctx);

	return ret;
}



#define SIDEBAR_W 200

//scr_w and scr_h are logical dimensions not raw pixels
int do_filebrowser(file_browser* fb, struct nk_context* ctx, int scr_w, int scr_h)
{
	int is_selected = SDL_FALSE;
	int symbol;
	int list_height;
	int active;
	float search_height = 0;
	int ret = 1;

	struct nk_rect bounds;
	const struct nk_input* in = &ctx->input;

	static struct nk_list_view lview, rview;
	static float header_ratios[] = {0.49f, 0.01f, 0.15f, 0.01f, 0.34f };
	static int splitter_down = 0;

	// TODO "Open Directory", "Open Folder"?
	const char* open_strs[] = { "Open", "Open Dir" };

#define UP_WIDTH 100
	float path_szs[2] = { 0, UP_WIDTH };

	int search_flags = NK_EDIT_FIELD | NK_EDIT_SIG_ENTER | NK_EDIT_GOTO_END_ON_ACTIVATE;
	int text_path_flags = NK_EDIT_SELECTABLE | NK_EDIT_CLIPBOARD | NK_EDIT_AUTO_SELECT;

	SDL_Event event = { .type = g->userevent };

	cvector_file* f = &fb->files;

	if (fb->file[0]) {
		// You've already selected a file why are you calling do_filebrowser?
		return 0;
	}

	if (!nk_input_is_mouse_down(in, NK_BUTTON_LEFT))
		splitter_down = 0;

	if (nk_begin(ctx, "File Selector", nk_rect(0, 0, scr_w, scr_h), NK_WINDOW_NO_SCROLLBAR)) {

		struct nk_rect win_content_rect = nk_window_get_content_region(ctx);
		struct nk_vec2 win_spacing = ctx->style.window.spacing;


		//printf("scr_w,scr_h = %d, %d\n%f %f\n", scr_w, scr_h, win_content_rect.w, win_content_rect.h);

		nk_layout_row_template_begin(ctx, 0);
		nk_layout_row_template_push_static(ctx, 100);
		nk_layout_row_template_push_dynamic(ctx);
		nk_layout_row_template_push_static(ctx, 100);
		nk_layout_row_template_end(ctx);
		if (nk_button_label(ctx, "Cancel")) {
			// TODO maybe just have a done flag in file browser?
			ret = 0;
		}

		search_height = nk_widget_bounds(ctx).h;
		// Search field
		// TODO
		//nk_button_label(ctx, "Search");
		active = nk_edit_string(ctx, search_flags, fb->text_buf, &fb->text_len, STRBUF_SZ, nk_filter_default);
		if (active & NK_EDIT_COMMITED && fb->text_len) {

			fb_search_filenames(fb);
			memset(&rview, 0, sizeof(rview));
			fb->is_search_results = TRUE;

			// use no selection to ignore the "Enter" in events so we don't exit
			// list mode.  Could add state to handle keeping the selection but meh
			fb->selection = -1;  // no selection among search
			nk_edit_unfocus(ctx);
		}

		// only enable "Open" button if you have a selection
		if (fb->selection < 0) {
			nk_widget_disable_begin(ctx);
		}
		if (nk_button_label(ctx, open_strs[fb->select_dir])) {
			if (f->a[fb->selection].size == -1 && !fb->select_dir) {
				switch_dir(fb, f->a[fb->selection].path);
			} else {
				strncpy(fb->file, f->a[fb->selection].path, MAX_PATH_LEN);
				ret = 0;
			}
		}
		nk_widget_disable_end(ctx);

		int path_rows = 1; // default 1 for text path
		// don't show path if recents or in root directory "/"
		if (!fb->is_recents && fb->dir[1]) {
			// method 1
			// breadcrumb buttons
			if (!fb->is_text_path) {
				int depth = 0; // number of breadcrumb buttons;

				ctx->style.window.spacing.x = 0;
				char *d = fb->dir;
				char *begin = d + 1;
				char tmp;
				nk_layout_row_dynamic(ctx, 0, 6);
				while (*d++) {
					tmp = *d;
					if (tmp == '/' || !tmp) {
						*d = '\0';
						if (nk_button_label(ctx, begin)) {
							switch_dir(fb, NULL);
							break;
						}
						depth++;
						if (tmp) *d = '/';
						begin = d + 1;
					}
				}
				path_rows = depth/6 + 1;
				ctx->style.window.spacing.x = win_spacing.x;
			} else {

				// 2 Methods of text path mode

				// method 2
				// TODO how to make this look like method 3, submit issue/documentation
				path_szs[0] = win_content_rect.w-win_spacing.x-UP_WIDTH;
				nk_layout_row(ctx, NK_STATIC, 0, 2, path_szs);

				
				// method 3
				/*
				nk_layout_row_template_begin(ctx, 0);
				nk_layout_row_template_push_dynamic(ctx);
				nk_layout_row_template_push_static(ctx, 100);
				nk_layout_row_template_end(ctx);
				*/
				
				int dir_len = strlen(fb->dir);
				nk_edit_string(ctx, text_path_flags, fb->dir, &dir_len, MAX_PATH_LEN, nk_filter_default);


				if (nk_button_label(ctx, "Up")) {
					char* s = strrchr(fb->dir, '/');
					if (s != fb->dir) {
						*s = 0;
						switch_dir(fb, NULL);
					} else {
						switch_dir(fb, "/");
					}
				}
			}
		}

		const float group_szs[] = { SIDEBAR_W, scr_w-SIDEBAR_W };

		nk_layout_row(ctx, NK_STATIC, scr_h, 2, group_szs);

		if (nk_group_begin(ctx, "Sidebar", 0)) {

			// Make dynamic array to add saved bookmarked locations
			nk_layout_row_dynamic(ctx, 0, 1);

			nk_label(ctx, "Settings", NK_TEXT_CENTERED);

			if (fb->num_exts) {
				//nk_checkbox_label(ctx, "All Files", &fb->ignore_exts);
				static const char* ext_opts[] = { FILE_TYPE_STR, "All Files" };
				struct nk_rect bounds = nk_widget_bounds(ctx);
				int old = fb->ignore_exts;
				fb->ignore_exts = nk_combo(ctx, ext_opts, NK_LEN(ext_opts), old, FONT_SIZE, nk_vec2(bounds.w, 300));
				if (fb->ignore_exts != old) {
					if (!fb->is_recents) {
						switch_dir(fb, NULL);
					} else {
						handle_recents(fb);
					}
				}
			}
			static const char* path_opts[] = { "Breadcrumbs", "Text" };
			struct nk_rect bounds = nk_widget_bounds(ctx);
			fb->is_text_path = nk_combo(ctx, path_opts, NK_LEN(path_opts), fb->is_text_path, FONT_SIZE, nk_vec2(bounds.w, 300));

			if (nk_checkbox_label(ctx, "Show Hidden", &fb->show_hidden)) {
				switch_dir(fb, NULL);
			}

			if (nk_checkbox_label(ctx, "Open Dir", &fb->select_dir)) {
				switch_dir(fb, NULL);
			}

			nk_label(ctx, "Bookmarks", NK_TEXT_CENTERED);
			if (fb->get_recents) {
				if (nk_button_label(ctx, "Recents")) {
					handle_recents(fb);
				}
			}
			if (nk_button_label(ctx, "Home")) {
				switch_dir(fb, fb->home);
			}
			if (nk_button_label(ctx, "Desktop")) {
				switch_dir(fb, fb->desktop);
			}
			if (nk_button_label(ctx, "Computer")) {
				switch_dir(fb, "/");
			}
			char** bmarks = g->bookmarks.a;
			for (int i=0; i<g->bookmarks.size; ++i) {
				char* b = bmarks[i];
				char* name = strrchr(b, '/');

				// handle Windows "C:/" correctly
				if (name == &b[2] && b[1] == ':') {
					name = &b[0];
				}
				if (name != b) {
					name++;
				}
				if (nk_button_label(ctx, name)) {
					switch_dir(fb, b);
				}
			}
			if (nk_button_label(ctx, "Add to Bookmarks")) {
				cvec_push_str(&g->bookmarks, fb->dir);
			}


			nk_group_end(ctx);
		}

		if (nk_group_begin(ctx, "List", 0)) {

			// main list column headers and splitters
			nk_layout_row(ctx, NK_DYNAMIC, 0, 5, header_ratios);

			symbol = NK_SYMBOL_NONE; // 0
			if (fb->sorted_state == NAME_UP)
				symbol = NK_SYMBOL_TRIANGLE_UP;
			else if (fb->sorted_state == NAME_DOWN)
				symbol = NK_SYMBOL_TRIANGLE_DOWN;

			// TODO name or path?
			if (nk_button_symbol_label(ctx, symbol, "Name", NK_TEXT_LEFT)) {
				event.user.code = SORT_NAME;
				SDL_PushEvent(&event);
			}

			bounds = nk_widget_bounds(ctx);
			nk_spacing(ctx, 1);
			if ((splitter_down == 1 || (nk_input_is_mouse_hovering_rect(in, bounds) && !splitter_down)) &&
				nk_input_is_mouse_down(in, NK_BUTTON_LEFT)) {
				float change = in->mouse.delta.x/(ctx->current->layout->bounds.w-8);
				header_ratios[0] += change;
				header_ratios[2] -= change;
				if (header_ratios[2] < 0.05f) {
					header_ratios[2] = 0.05f;
					header_ratios[0] = 0.93f - header_ratios[4];
				} else if (header_ratios[0] < 0.05f) {
					header_ratios[0] = 0.05f;
					header_ratios[2] = 0.93f - header_ratios[4];
				}
				splitter_down = 1;
			}

			// I hate redundant logic but the alternative is repeated gui code
			// TODO think of a better way
			symbol = NK_SYMBOL_NONE; // 0
			if (fb->sorted_state == SIZE_UP)
				symbol = NK_SYMBOL_TRIANGLE_UP;
			else if (fb->sorted_state == SIZE_DOWN)
				symbol = NK_SYMBOL_TRIANGLE_DOWN;

			if (nk_button_symbol_label(ctx, symbol, "Size", NK_TEXT_LEFT)) {
				event.user.code = SORT_SIZE;
				SDL_PushEvent(&event);
			}

			bounds = nk_widget_bounds(ctx);
			nk_spacing(ctx, 1);
			if ((splitter_down == 2 || (nk_input_is_mouse_hovering_rect(in, bounds) && !splitter_down)) &&
				nk_input_is_mouse_down(in, NK_BUTTON_LEFT)) {
				float change = in->mouse.delta.x/(ctx->current->layout->bounds.w-8);
				header_ratios[2] += change;
				header_ratios[4] -= change;
				if (header_ratios[2] < 0.05f) {
					header_ratios[2] = 0.05f;
					header_ratios[4] = 0.93f - header_ratios[0];
				} else if (header_ratios[4] < 0.05f) {
					header_ratios[4] = 0.05f;
					header_ratios[2] = 0.93f - header_ratios[0];
				}
				splitter_down = 2;
			}

			symbol = NK_SYMBOL_NONE; // 0
			if (fb->sorted_state == MODIFIED_UP)
				symbol = NK_SYMBOL_TRIANGLE_UP;
			else if (fb->sorted_state == MODIFIED_DOWN)
				symbol = NK_SYMBOL_TRIANGLE_DOWN;

			if (nk_button_symbol_label(ctx, symbol, "Modified", NK_TEXT_LEFT)) {
				event.user.code = SORT_MODIFIED;
				SDL_PushEvent(&event);
			}

			float ratios[] = { header_ratios[0]+0.01f, header_ratios[2], header_ratios[4]+0.01f };
			
			// path_rows is 1 for text mode or >=1 for breadcrumb mode, +2 for
			// the search bar and the column header buttons
			nk_layout_row_dynamic(ctx, scr_h-(path_rows+2)*search_height, 1);


			if (fb->is_search_results) {
				if (!fb->search_results.size) {
					if (nk_button_label(ctx, "No matching results")) {
						fb->is_search_results = FALSE;
						fb->text_buf[0] = 0;
						fb->text_len = 0;
						fb->selection = -1;
						fb->list_setscroll = TRUE;
					}
				} else {
					if (nk_list_view_begin(ctx, &rview, "Result List", NK_WINDOW_BORDER, FONT_SIZE+16, fb->search_results.size)) {
						nk_layout_row(ctx, NK_DYNAMIC, 0, 3, ratios);
						int i;
						for (int j=rview.begin; j<rview.end; ++j) {
							i = fb->search_results.a[j];
							// TODO Do I really need fb->selection?  Can I use g->img[0].index (till I get multiple selection)
							// also thumb_sel serves the same/similar purpose
							is_selected = fb->selection == j;
							if (nk_selectable_label(ctx, f->a[i].name, NK_TEXT_LEFT, &is_selected)) {
								if (is_selected) {
									fb->selection = j;
								} else {
									// could support unselecting, esp. with CTRL somehow if I ever allow
									// multiple selection
									// fb->selection = -1;

									// for now, treat clicking a selection as a "double click" ie same as return
									if (f->a[i].size == -1) {
										switch_dir(fb, f->a[i].path);
										break;
									} else {
										strncpy(fb->file, f->a[i].path, MAX_PATH_LEN);
										ret = 0;
									}
									//break; //?
								}
							}
							nk_label(ctx, f->a[i].size_str, NK_TEXT_RIGHT);
							nk_label(ctx, f->a[i].mod_str, NK_TEXT_RIGHT);
						}
						list_height = ctx->current->layout->clip.h; // ->bounds.h?
						nk_list_view_end(&rview);
					}
				}
				if (fb->list_setscroll && rview.total_height > list_height) {
					int scroll_limit = rview.total_height - list_height; // little off
					nk_uint y = (fb->selection/(float)(fb->search_results.size-1) * scroll_limit) + 0.999f;
					nk_group_set_scroll(ctx, "Result List", 0, y);
					fb->list_setscroll = FALSE;
				}
			} else {
				if (nk_list_view_begin(ctx, &lview, "File List", NK_WINDOW_BORDER, FONT_SIZE+16, f->size)) {
					// TODO ratio layout 0.5 0.2 0.3 ? give or take
					//nk_layout_row_dynamic(ctx, 0, 3);
					nk_layout_row(ctx, NK_DYNAMIC, 0, 3, ratios);
					for (int i=lview.begin; i<lview.end; ++i) {
						assert(i < f->size);
						is_selected = fb->selection == i;
						if (nk_selectable_label(ctx, f->a[i].name, NK_TEXT_LEFT, &is_selected)) {
							if (is_selected) {
								fb->selection = i;
							} else {
								if (f->a[i].size == -1) {
									switch_dir(fb, f->a[i].path);
									break;
								} else {
									strncpy(fb->file, f->a[i].path, MAX_PATH_LEN);
									ret = 0;
								}
							}
						}
						nk_label(ctx, f->a[i].size_str, NK_TEXT_RIGHT);
						nk_label(ctx, f->a[i].mod_str, NK_TEXT_RIGHT);
					}
					list_height = ctx->current->layout->clip.h; // ->bounds.h?
					nk_list_view_end(&lview);
				}

				if (fb->list_setscroll && lview.total_height > list_height) {
					int scroll_limit = lview.total_height - list_height; // little off
					nk_uint y = (fb->selection/(float)(f->size-1) * scroll_limit) + 0.999f;
					nk_group_set_scroll(ctx, "File List", 0, y);
					fb->list_setscroll = FALSE;
				}
			}
			nk_group_end(ctx);
		}
	}
	nk_end(ctx);

	return ret;
}



char* uri_decode(const char* str)
{
	int len = strlen(str);
	char* dst = malloc(len+1);
	int x, a, b;
	char hex[3] = {0};
	int j = 0;
	
	for (int i=0; str[i]; ++i) {
		if (str[i] == '%') {
			a = str[i+1];
			b = str[i+2];
			if (!a) {
				// copy %?
				return dst;
			}
			if (isxdigit(a) && isxdigit(b)) {
				hex[0] = a;
				hex[1] = b;
				x = strtol(hex, NULL, 16);
				dst[j++] = x;
				i += 2;
			}
		} else if (str[i] == '&') {
			if (!strncmp(str+i+1, "apos;", 5)) {
				dst[j++] = '\'';
				i += 5;
			} else if (!strncmp(str+i+1, "amp;", 4)) {
				dst[j++] = '&';
				i += 4;
			} else {
				dst[j++] = str[i];
			}
		} else {
			dst[j++] = str[i];
		}
	}
	dst = realloc(dst, j+1);
	dst[j] = 0;

	return dst;
}

/** Read file into into allocated string, return in *out
 * Data is NULL terminated.  file is closed before returning (since you
 * just read the entire file ...). */
int file_read(FILE* file, char** out)
{
	assert(file);
	assert(out);

	char* data = NULL;
	long size;

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	if (size <= 0) {
		if (size == -1)
			perror("ftell failure");
		fclose(file);
		return 0;
	}

	data = (char*)malloc(size+1);
	if (!data) {
		fclose(file);
		return 0;
	}

	rewind(file);
	if (!fread(data, size, 1, file)) {
		perror("fread failure");
		fclose(file);
		free(data);
		return 0;
	}

	data[size] = 0; /* null terminate in all cases even if reading binary data */

	*out = data;

	fclose(file);
	return size;
}

int linux_recents(cvector_str* recents, void* userdata)
{
	UNUSED(userdata);

	assert(recents);

	char path[MAX_PATH_LEN];
	snprintf(path, MAX_PATH_LEN, "%s/.local/share/recently-used.xbel", get_homedir());
	FILE* recents_file = fopen(path, "r");
	if (!recents_file) {
		perror("Could not open recents file");
		return 0;
	}

	// TODO cvector_str* or char***, obviously leaning toward former
	cvec_reserve_str(recents, 200);

	char* text = NULL;
	int file_len = 0;

	if (!(file_len = file_read(recents_file, &text))) {
		// empty file
		return 0;
	}

	const char needle[] = "bookmark href=\"file://";
	int needle_len = strlen(needle);

	char* start_search = text;

	char* result = NULL;
	char* end;
	char* dst;
	int i = 0;
	while ((result = strstr(start_search, needle))) {
		result += needle_len;
		end = strchr(result, '\"');

		*end = 0;

		dst = uri_decode(result);
		//printf("%d %s\n%d %s\n", i, result, i, dst);
		cvec_pushm_str(recents, dst);

		// continue search just past the end of last result
		start_search = end+1;
		i++;
	}

	free(text);

	// unnecessary with size included in cvector_str...
	return i;
}

int windows_recents(cvector_str* recents, void* userdata)
{
	UNUSED(userdata);

	assert(recents);

	char recents_dir_buf[STRBUF_SZ];
	int ret = snprintf(recents_dir_buf, STRBUF_SZ, "%s\\Microsoft\\Windows\\Recent", getenv("APPDATA"));
	if (ret >= STRBUF_SZ) {
		return 0;
	}

	// TODO could just use scandir or inline opendir/readdir since all the extra file work is wasted...
	// Or have windows and lnk files be a special case and just switch to the directory but when they
	// click on a lnk file extract the path right then...?
	cvector_file links = {0};
	const char* exts[] = { ".lnk" }; // shouldn't be anything else in the directory but jic

	fb_scandir(&links, recents_dir_buf, exts, 1, SDL_FALSE, SDL_FALSE);

	char* tmp;
	for (int i=0; i<links.size; ++i) {
		if ((tmp = clnk_get_path(links.a[i].path))) {
			normalize_path(tmp);
			cvec_pushm_str(recents, tmp);
		}
	}
	cvec_free_file(&links);
	return recents->size;
}






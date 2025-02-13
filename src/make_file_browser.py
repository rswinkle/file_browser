# generate all in one file_browser.h from component libraries/files

import sys, os, glob, argparse



open_header = """
#ifndef FILE_BROWSER_H
#define FILE_BROWSER_H


#ifdef __cplusplus
extern "C" {
#endif

"""

close_header = """
#ifdef __cplusplus
}
#endif

// end FILE_BROWSER_H
#endif

"""

# TODO make this more robust
def find_include(s, *args):
    start = 0
    if args:
        start = args[0]
    inc_start = s.find('#include "', start)

    if inc_start != -1:
        fname_start = inc_start+10
        fname_end = s.find('"', fname_start)
        fname = s[fname_start:fname_end]
        return [inc_start, fname_end+1, fname]
    else:
        return None

def replace_user_includes(s):
    ret = find_include(s)
    #print(ret)
    while ret:
        fname = ret[2]
        print("replacing", fname)
        s = s.replace('#include "{}"'.format(fname), open(fname, 'r').read(), 1)
        ret = find_include(s)

    return s

def replace_user_include_c(s):
    ret = find_include(s)
    #print(ret)
    while ret:
        fname = ret[2]
        if fname.endswith(".c"):
            print("replacing c", fname)
            s = s.replace('#include "{}"'.format(fname), open(fname, 'r').read(), 1)
        else:
            # should have been taken care of in the header portion so eliminate
            print("replacing c", fname)
            s = s.replace('#include "{}"'.format(fname), '', 1)

        ret = find_include(s)

    return s


if __name__ == "__main__":
    fb_h = open("file_browser.h", "w")

    fb_h.write("/*\n")

    fb_h.write(open("header_docs.txt").read())

    # for now put this here
    fb_h.write("\n")
    fb_h.write(open("../LICENSE").read())

    fb_h.write("*/\n")

    fb_h.write(open_header)

    # Try doing this another way
    fb_h_str = open("filebrowser.h", "r").read()

    
    fb_h_str = replace_user_includes(fb_h_str)
    #print(fb_h_str)
    fb_h.write(fb_h_str)

    fb_h.write(close_header)

    fb_impl = open("fb_cvector.c", "r").read()
    #fb_impl = replace_user_includes(fb_impl)

    fb_impl += open("file.c", "r").read()
    fb_impl += open("filebrowser.c", "r").read()

    fb_impl = replace_user_include_c(fb_impl)

    fb_h.write("\n#ifdef FILE_BROWSER_IMPLEMENTATION\n\n")
    #fb_h.write("\n#define CVECTOR_IMPLEMENTATION\n\n")
    
    fb_h.write(fb_impl)

    #fb_h.write("#undef CVECTOR_IMPLEMENTATION\n")
    fb_h.write("#undef FILE_BROWSER_IMPLEMENTATION\n")

    fb_h.write("#endif\n")

    fb_h.close()

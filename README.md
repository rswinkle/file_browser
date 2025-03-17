file_browser
============

A single header library to do file browsing for file/folder selection in
C that can be used with any UI system.

Features:

* Sorting based on file name, size, last modified
* Filtering based on a provided list of extensions (on/off toggle)
* Searching the current directory
* Show/Hide hidden files (beginning with .)
* Folder selection mode (only show directories)
* Supports Recent Files (user provided function, see Nuklear Demo)
* UI library agnostic

The two demos are a terminal UI meant more to be a proof of concept and one
that uses the [Nuklear GUI library](https://github.com/Immediate-Mode-UI/Nuklear)
with the SDL_renderer backend that showcases all the features.

Gallery
=======
![Nuklear demo](https://raw.githubusercontent.com/rswinkle/file_browser/main/media/Nuklear_filebrowser.png)
![Terminal demo](https://raw.githubusercontent.com/rswinkle/file_browser/main/media/terminal_filebrowser.png)


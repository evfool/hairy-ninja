hairy-ninja
===========

Playground for GTK3 and SDL2 integration.

GTK3 for 2d game development could do better. I have tried several options:
* cairo just isn't meant to be for fluid 2d animation
* clutter and clutter-gtk are lagging (see https://github.com/evfool/tripping-ninja)
* pygame had relatively good support for python 2.x and gtk2.x, but that doesn't help in the era of python3 and gtk3
* with SDL1.2 we had a window_id hack, which does not work with the freshly released SDL2, probably due to the multi-window feature in SDL2
I am not aware of any other options for GL/accelerated 2D drawing in gtk+3 (maybe except of an in-progress GtkGlAreaExt, I think), please let me know if you know about anything else that could be used for 2D game development INSIDE a GTK+ window (with GTK+3 UI, toolbars, buttons, sidebar, etc.)

As SDL2 got released recently, I thought I'd give it a try.

After setting up SDL2 on ubuntu from source (thanks to http://nothingtocode.blogspot.ro/2013/07/setting-up-sdl2-in-ubuntu-or-linux-mint.html) and creating an Anjuta Gtk3 project with SDL2 and X11 library set up as custom libraries, and with a bit of plumbing (merging the Gtk3 sample project with the SDL2 sample from the SDL2 setup instructions on Ubuntu) I have a running example of SDL2 rendering inside a Gtk window.
It's ugly, it's not perfect, I have only managed to run it from inside anjuta for now (because of some autotools stuff), but it works from Anjuta.


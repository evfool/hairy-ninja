## Process this file with automake to produce Makefile.in

## Created by Anjuta


uidir = $(pkgdatadir)/ui
ui_DATA = gtk3_sdl2.ui


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(GTK3_SDL2_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gtk3_sdl2

gtk3_sdl2_SOURCES = \
	main.c \
	gtk3-sdl2.h \
	gtk3-sdl2.c	

gtk3_sdl2_LDFLAGS = \
	-Wl,--export-dynamic

gtk3_sdl2_LDADD = $(GTK3_SDL2_LIBS)


EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)


## Process this file with automake to produce Makefile.in

## Created by Anjuta


uidir = $(pkgdatadir)/ui
ui_DATA = gtk3_sdl2.ui sdl_logo.bmp ship.svg


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
	gtk3-sdl2.c \
	ftgrays.c \
	ftgrays.h \
	ftimage.h \
	internals.h \
	matrix.c \
	render.c \
	SDL_svg.c \
	SDL_svg.h \
	libsvg/config.h \
	libsvg/svg.c \
	libsvg/svg.h \
	libsvg/svg_ascii.c \
	libsvg/svg_ascii.h \
	libsvg/svg_attribute.c \
	libsvg/svg_color.c \
	libsvg/svg_element.c \
	libsvg/svg_gradient.c \
	libsvg/svg_group.c \
	libsvg/svg_image.c \
	libsvg/svgint.h \
	libsvg/svg_length.c \
	libsvg/svg_paint.c \
	libsvg/svg_parser.c \
	libsvg/svg_path.c \
	libsvg/svg_pattern.c \
	libsvg/svg_str.c \
	libsvg/svg_style.c \
	libsvg/svg_text.c \
	libsvg/svg_transform.c \
	libsvg/svg_version.h	

gtk3_sdl2_CFLAGS = -I/usr/include/libxml2 

gtk3_sdl2_LDFLAGS = \
	-Wl,--export-dynamic

gtk3_sdl2_LDADD = $(GTK3_SDL2_LIBS) \
	-lm \
	-ljpeg


EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)


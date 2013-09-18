## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src 

gtk3_sdl2docdir = ${prefix}/doc/gtk3_sdl2
gtk3_sdl2doc_DATA = \
	README\
	COPYING\
	AUTHORS\
	ChangeLog\
	INSTALL\
	NEWS


EXTRA_DIST = $(gtk3_sdl2doc_DATA)


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(gtk3_sdl2docdir)

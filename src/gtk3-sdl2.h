/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * gtk3-sdl2.h
 * Copyright (C) 2013 evfool <evfool@evfool-Inspiron-560>
 * 
 * gtk3-sdl2 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * gtk3-sdl2 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _GTK3_SDL2_
#define _GTK3_SDL2_

#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <gdk/gdkx.h>
#include "SDL_svg.h"

G_BEGIN_DECLS

#define GTK3_SDL2_TYPE_APPLICATION             (gtk3_sdl2_get_type ())
#define GTK3_SDL2_APPLICATION(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK3_SDL2_TYPE_APPLICATION, Gtk3sdl2))
#define GTK3_SDL2_APPLICATION_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK3_SDL2_TYPE_APPLICATION, Gtk3sdl2Class))
#define GTK3_SDL2_IS_APPLICATION(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK3_SDL2_TYPE_APPLICATION))
#define GTK3_SDL2_IS_APPLICATION_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK3_SDL2_TYPE_APPLICATION))
#define GTK3_SDL2_APPLICATION_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK3_SDL2_TYPE_APPLICATION, Gtk3sdl2Class))

typedef struct _Gtk3sdl2Class Gtk3sdl2Class;
typedef struct _Gtk3sdl2 Gtk3sdl2;
typedef struct _Gtk3sdl2Private Gtk3sdl2Private;

struct _Gtk3sdl2Class
{
	GtkApplicationClass parent_class;
};

struct _Gtk3sdl2
{
	GtkApplication parent_instance;

	Gtk3sdl2Private *priv;

};

GType gtk3_sdl2_get_type (void) G_GNUC_CONST;
Gtk3sdl2 *gtk3_sdl2_new (void);

/* Callbacks */

G_END_DECLS

#endif /* _APPLICATION_H_ */


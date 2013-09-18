/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * gtk3-sdl2.c
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
#include "gtk3-sdl2.h"

/* For testing propose use the local (not installed) ui file */
//#define UI_FILE PACKAGE_DATA_DIR"/ui/gtk3_sdl2.ui"
#define LOGO_BMP "src/sdl_logo.bmp"
#define UI_FILE "src/gtk3_sdl2.ui"
#define TOP_WINDOW "window"
#define SDL_AREA "sdl2_area"


G_DEFINE_TYPE (Gtk3sdl2, gtk3_sdl2, GTK_TYPE_APPLICATION);


/* Define the private structure in the .c file */
#define GTK3_SDL2_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), GTK3_SDL2_TYPE_APPLICATION, Gtk3sdl2Private))

/* The window */
SDL_Window* sdl_window = NULL;
	
/* The window surface */
SDL_Surface* sdl_screen = NULL;

SDL_Surface* sdl_image = NULL;

guint idle_handler;

struct _Gtk3sdl2Private
{
	/* ANJUTA: Widgets declaration for gtk3_sdl2.ui - DO NOT REMOVE */
	GtkWidget *sdl_area;
	
};

static void
draw_sdl (gpointer user_data)
{
  SDL_BlitSurface( sdl_image, NULL, sdl_screen, NULL );
  SDL_UpdateWindowSurface( sdl_window );
}

static void
setup_sdl (Window x11_window)
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
  {
    g_debug( "SDL2 could not initialize! SDL2_Error: %s\n", SDL_GetError() );
  }
  else
  {
    sdl_window = SDL_CreateWindowFrom((const void*)
        x11_window);
    sdl_screen = SDL_GetWindowSurface( sdl_window );
    sdl_image = SDL_LoadBMP( LOGO_BMP );
    idle_handler = g_idle_add ( draw_sdl, NULL);
  }
}

static void
cleanup_sdl (GtkApplication *app, gpointer user_data)
{
	SDL_FreeSurface( sdl_image );
  //SDL_DestroyWindow( sdl_window );
  //SDL_Quit(); 
}

static void
area_resized (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
  g_source_remove (idle_handler);
  idle_handler = g_idle_add ( draw_sdl, NULL);
  draw_sdl (user_data);
}

/* Create a new window loading a file */
static void
gtk3_sdl2_new_window (GApplication *app,
                      GFile        *file)
{
	GtkWidget *window;
  
	GdkWindow *gdk_window;
	
	GtkBuilder *builder;
	GError* error = NULL;

	Gtk3sdl2Private *priv = GTK3_SDL2_GET_PRIVATE(app);

	/* Load UI from file */
	builder = gtk_builder_new ();
	if (!gtk_builder_add_from_file (builder, UI_FILE, &error))
	{
		g_critical ("Couldn't load builder file: %s", error->message);
		g_error_free (error);
	}

	/* Auto-connect signal handlers */
	gtk_builder_connect_signals (builder, app);

	/* Get the window object from the ui file */
	window = GTK_WIDGET (gtk_builder_get_object (builder, TOP_WINDOW));
        if (!window)
        {
		g_critical ("Widget \"%s\" is missing in file %s.",
				TOP_WINDOW,
				UI_FILE);
        }

	
	/* ANJUTA: Widgets initialization for gtk3_sdl2.ui - DO NOT REMOVE */
  priv->sdl_area = GTK_WIDGET (gtk_builder_get_object (builder, SDL_AREA));
  g_signal_connect (G_OBJECT (priv->sdl_area), "configure-event", 
                    G_CALLBACK (area_resized), app);
	gdk_window = gtk_widget_get_window(priv->sdl_area);
  // fail if we're not on X11, as SDL2 does not support 
	// anything else (aka Wayland) on Linux AFAIK
	g_assert (GDK_IS_X11_WINDOW(gdk_window));

	setup_sdl (gdk_x11_window_get_xid (GDK_X11_WINDOW (gdk_window)));
		
	g_object_unref (builder);
	
	
	gtk_window_set_application (GTK_WINDOW (window), GTK_APPLICATION (app));
	if (file != NULL)
	{
		/* TODO: Add code here to open the file in the new window */
	}
	gtk_widget_show_all (GTK_WIDGET (window));
}



/* GApplication implementation */
static void
gtk3_sdl2_activate (GApplication *application)
{
	gtk3_sdl2_new_window (application, NULL);
}

static void
gtk3_sdl2_open (GApplication  *application,
                     GFile        **files,
                     gint           n_files,
                     const gchar   *hint)
{
	gint i;

	for (i = 0; i < n_files; i++)
		gtk3_sdl2_new_window (application, files[i]);
}

static void
gtk3_sdl2_init (Gtk3sdl2 *object)
{

}

static void
gtk3_sdl2_finalize (GObject *object)
{
	G_OBJECT_CLASS (gtk3_sdl2_parent_class)->finalize (object);
}

static void
gtk3_sdl2_class_init (Gtk3sdl2Class *klass)
{
	G_APPLICATION_CLASS (klass)->activate = gtk3_sdl2_activate;
	G_APPLICATION_CLASS (klass)->open = gtk3_sdl2_open;

	g_type_class_add_private (klass, sizeof (Gtk3sdl2Private));

	G_OBJECT_CLASS (klass)->finalize = gtk3_sdl2_finalize;
}

Gtk3sdl2 *
gtk3_sdl2_new (void)
{
	Gtk3sdl2 * result = g_object_new (gtk3_sdl2_get_type (),
	                     "application-id", "org.gnome.gtk3_sdl2",
	                     "flags", G_APPLICATION_HANDLES_OPEN,
	                     NULL);
	g_signal_connect (G_OBJECT (result), "shutdown", G_CALLBACK (cleanup_sdl), NULL);
	return result;
}


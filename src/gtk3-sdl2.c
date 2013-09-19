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
#define UI_FILE PACKAGE_DATA_DIR"/ui/gtk3_sdl2.ui"
#define LOGO_BMP PACKAGE_DATA_DIR"/ui/sdl_logo.bmp"
#define SHIP_SVG PACKAGE_DATA_DIR"/ui/ship.svg"
//#define SHIP_SVG "src/ship.svg"
//#define LOGO_BMP "src/sdl_logo.bmp"
//#define UI_FILE "src/gtk3_sdl2.ui"
#define TOP_WINDOW "window"
#define SDL_AREA "sdl2_area"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 60
//#define BLITTING 0
//#define RENDERER 1

G_DEFINE_TYPE (Gtk3sdl2, gtk3_sdl2, GTK_TYPE_APPLICATION);


/* Define the private structure in the .c file */
#define GTK3_SDL2_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), GTK3_SDL2_TYPE_APPLICATION, Gtk3sdl2Private))

struct _Gtk3sdl2Private
{
	/* ANJUTA: Widgets declaration for gtk3_sdl2.ui - DO NOT REMOVE */
	GtkWidget *sdl_area;
	
  /* The window */
  SDL_Window *sdl_window;
  SDL_Surface *sdl_image;
  guint width;
	guint height;
	guint x;
	guint y;
	
	/*Renderer specific*/
#if defined(RENDERER)
  SDL_Renderer *sdl_renderer;
  SDL_Texture *sdl_texture;
#endif
  /*Simple blitting specific*/
	SDL_Surface *sdl_screen;

  guint idle_handler;	
};

static gboolean
draw_sdl (gpointer user_data)
{
  SDL_Rect dest_rect;
  Gtk3sdl2Private *priv = GTK3_SDL2_GET_PRIVATE (G_APPLICATION (user_data));
  
  dest_rect.w = priv->sdl_image->w;
  dest_rect.h = priv->sdl_image->h;

  
  dest_rect.x = priv->width/2-priv->sdl_image->w/2;
  dest_rect.y = priv->height/2-priv->sdl_image->h/2;

#if defined(RENDERER)
	SDL_RenderClear(priv->sdl_renderer);
  SDL_RenderCopy(priv->sdl_renderer, priv->sdl_texture, NULL, &dest_rect);
  SDL_RenderPresent(priv->sdl_renderer);
#else 
	//SDL_Color black  = {0,0,0};
	SDL_FillRect (priv->sdl_screen, NULL, NULL);
  SDL_BlitSurface (priv->sdl_image, NULL, priv->sdl_screen, &dest_rect);
  SDL_UpdateWindowSurface (priv->sdl_window); 
#endif
  return TRUE;
}

static void
setup_sdl (GApplication *app)
{
  Gtk3sdl2Private *priv = GTK3_SDL2_GET_PRIVATE (app);
  
	if( SDL_Init (SDL_INIT_VIDEO ) < 0 )
  {
    g_debug ("SDL2 could not initialize! SDL2_Error: %s\n", SDL_GetError());
  }
  else
  {
		if (priv->sdl_window == NULL)
		  {
        GdkWindow *gdk_window = gtk_widget_get_window (priv->sdl_area);
        Window x11_window = gdk_x11_window_get_xid (GDK_X11_WINDOW (gdk_window));
		    priv->sdl_window = SDL_CreateWindowFrom ( (const void*)x11_window);
			}
		else
		  {
				SDL_SetWindowSize (priv->sdl_window, priv->width, priv->height);
				SDL_ShowWindow (priv->sdl_window);
			}
		if (priv->sdl_image == NULL) 
		  {
				priv->sdl_image = SDL_LoadBMP ( LOGO_BMP );
			}
#if defined(RENDERER)
		if (priv->sdl_renderer == NULL)
		{
		  printf ("Initializing renderer\n");
		  guint i;
      guint num = SDL_GetNumRenderDrivers();
      for (i=0;i<num;i++) {
        SDL_RendererInfo info;
        SDL_GetRenderDriverInfo(i, &info);
        printf ("Renderer %d: %s\n",i, info.name);
      }
      guint window_flags = SDL_GetWindowFlags(priv->sdl_window);
      if (!(window_flags & SDL_WINDOW_OPENGL)) {
        printf ("Window doesn't support OpenGL, using software rendering\n");
        priv->sdl_renderer = SDL_CreateRenderer(priv->sdl_window, 
                                              -1, 
                                              SDL_RENDERER_SOFTWARE
                                              | SDL_RENDERER_TARGETTEXTURE);
      } else {
        priv->sdl_renderer = SDL_CreateRenderer(priv->sdl_window, 
                                              -1, 
                                              SDL_RENDERER_ACCELERATED
                                              | SDL_RENDERER_TARGETTEXTURE);
		  }
		}
		if (priv->sdl_texture == NULL)
		{
			priv->sdl_texture = SDL_CreateTextureFromSurface (priv->sdl_renderer,
                                                       priv->sdl_image);
		}
#elif defined(BLITTING)
		if (priv->sdl_screen == NULL)
		{
			printf ("Initializing blitting\n");
			priv->sdl_screen = SDL_GetWindowSurface (priv->sdl_window);
		}
#else
		if (priv->sdl_screen == NULL)
		{
			printf ("Initializing separate window\n");

			gint x, y;
			gdk_window_get_origin (gtk_widget_get_window (priv->sdl_area), &x, &y);
		
			priv->sdl_window = SDL_CreateWindow ("SDL Window", x, y, 
				                                   priv->width, priv->height, 
				                                   SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
			priv->sdl_screen = SDL_GetWindowSurface (priv->sdl_window);
		}
#endif
    
    priv->idle_handler = g_timeout_add (1000/FPS, draw_sdl, (gpointer)app);
  }
}

static void
cleanup_sdl (GtkApplication *app)
{
  Gtk3sdl2Private *priv = GTK3_SDL2_GET_PRIVATE (app);

  SDL_FreeSurface (priv->sdl_image);
	priv->sdl_image = NULL;
#if defined(RENDERER)
	SDL_DestroyTexture (priv->sdl_texture);
	priv->sdl_texture = NULL;
  SDL_DestroyRenderer (priv->sdl_renderer);
	priv->sdl_renderer = NULL;
#endif
	SDL_FreeSurface (priv->sdl_screen);
	priv->sdl_screen = NULL;
  SDL_DestroyWindow (priv->sdl_window);
	priv->sdl_window = NULL;
}

static void
area_resized (GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
  GApplication *app = G_APPLICATION (user_data);

	Gtk3sdl2Private *priv = GTK3_SDL2_GET_PRIVATE (app);
	guint width = gtk_widget_get_allocated_width (priv->sdl_area);
	guint height = gtk_widget_get_allocated_height (priv->sdl_area);
  gint x, y;
	gdk_window_get_origin (gtk_widget_get_window (priv->sdl_area), &x, &y);
			
	if (width != priv->width || height !=priv->height)
	  {
			printf ("Resized\n");
      g_source_remove (priv->idle_handler);
			
			priv->width = width;
		  priv->height = height;
			cleanup_sdl (app);
			setup_sdl (app);
      draw_sdl ( G_APPLICATION (user_data));
		  
	  } 
	else if (x != priv->x || y != priv->y) 
	  {
			printf ("Moved\n");
			priv->x = x;
			priv->y = y;
#if (!defined(RENDERER) && !defined(BLITTING))
			SDL_SetWindowPosition (priv->sdl_window, x, y);
			SDL_ShowWindow (priv->sdl_window);
#endif
	  }
}

static void
area_moved (GtkWidget *widget, GdkRectangle *rect, gpointer user_data)
{
  GApplication *app = G_APPLICATION (user_data);

	Gtk3sdl2Private *priv = GTK3_SDL2_GET_PRIVATE (app);
	gint x, y;
	gdk_window_get_origin (gtk_widget_get_window (priv->sdl_area), &x, &y);
			
	if (x != priv->x || y != priv->y) 
	  {
			printf ("Moved\n");
			priv->x = x;
			priv->y = y;
#if (!defined(RENDERER) && !defined(BLITTING))
			SDL_SetWindowPosition (priv->sdl_window, x, y);
			SDL_ShowWindow (priv->sdl_window);
#endif
	  }
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
	
	gtk_widget_set_size_request (priv->sdl_area, priv->width, priv->height);
	g_signal_connect (G_OBJECT (priv->sdl_area), "size-allocated", 
	                  G_CALLBACK (area_moved), app);
	g_signal_connect (G_OBJECT (priv->sdl_area), "configure-event", 
                    G_CALLBACK (area_resized), app);
	gdk_window = gtk_widget_get_window(priv->sdl_area);
  // fail if we're not on X11, as SDL2 does not support 
	// anything else (aka Wayland) on Linux AFAIK
	g_assert (GDK_IS_X11_WINDOW(gdk_window));

	setup_sdl (app);
		
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
  Gtk3sdl2Private *priv = GTK3_SDL2_GET_PRIVATE(object);
  priv->sdl_window = NULL;
  priv->sdl_image = NULL;
  priv->width = WINDOW_WIDTH;
	priv->height = WINDOW_HEIGHT;
	priv->x = 0;
	priv->y = 0;
  priv->idle_handler = 0;
#if defined(RENDERER)
	priv->sdl_renderer = NULL;
  priv->sdl_texture = NULL;
#endif
	priv->sdl_screen = NULL;

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
	g_type_init ();
	Gtk3sdl2 * result = g_object_new (gtk3_sdl2_get_type (),
	                     "application-id", "org.gnome.gtk3_sdl2",
	                     "flags", G_APPLICATION_HANDLES_OPEN,
	                     NULL);
	g_signal_connect (G_OBJECT (result), "shutdown", G_CALLBACK (cleanup_sdl), NULL);
	return result;
}


#include <gtk/gtk.h>
//p2p audio application based on gstreamer and gtk
/*EDW TA PIPELINES FTIAXNONTAI ME TON eukolo TROPO δηλαδη περνιουνται σαν φράσεις*/
/*gcc pipeline.c -o pipeline `pkg-config --cflags --libs gtk+-2.0 gstreamer-interfaces-0.10 gstreamer-0.10`
*/

#include <gst/gst.h>
#include <string.h>
  
#include <gtk/gtk.h>
#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <gdk/gdk.h>
#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk/gdkquartz.h>
#endif

GstElement *pipeline1,*pipeline2,*pipeline3;

/* This function is called when the GUI toolkit creates the physical window that will hold the video.
 * At this point we can retrieve its handler (which has a different meaning depending on the windowing system)
 * and pass it to GStreamer through the XOverlay interface. */
  static void realize_cb (GtkWidget *widget) {
  GdkWindow *window = gtk_widget_get_window (widget);
  guintptr window_handle;
  
  if (!gdk_window_ensure_native (window))
    g_error ("Couldn't create native window needed for GstXOverlay!");
  
  /* Retrieve window handler from GDK */
#if defined (GDK_WINDOWING_WIN32)
  window_handle = (guintptr)GDK_WINDOW_HWND (window);
#elif defined (GDK_WINDOWING_QUARTZ)
  window_handle = gdk_quartz_window_get_nsview (window);
#elif defined (GDK_WINDOWING_X11)
  window_handle = GDK_WINDOW_XID (window);
#endif

}

  GtkWidget *volume_button;
  GError *error = NULL;
  GtkBuilder *builder;
  GObject *window;
  GObject *button;
  GObject *submit;
  GObject *label6;
  GObject *label7;
  GObject *entry1;
  GObject *entry2;
  GObject *entry_text1;
  GObject *entry_text2;
  GtkWidget *main_hbox; 
  GtkEntry *ip_entry;
  GtkEntry *port_entry;
  const gchar *ip;
  const gchar *port;
 
  GObject *context_object;
  const gchar *pl1 = "alsasrc buffer-time=20000 ! alsasink buffer-time=20000 -v "; 
  gchar *pl2;
  const gchar *pl3 = "udpsrc port=7151 ! application/x-rtp,media=audio, clock-rate=88200, encoding-name=L16, payload=96 ! rtpL16depay ! audioconvert ! alsasink latency-time=20000  buffer-time=20000 ";
  

  const gchar *pl4 = "alsasrc buffer-time=20000 ! wavescope ! ffmpegcolorspace ! autovideosink ";
  const gchar *pl5 = "alsasrc buffer-time=20000 ! wavescope ! ffmpegcolorspace ! autovideosink ";
	

  void volume_button_value_changed_cb (GtkScaleButton * button, gdouble value);








//----------------------------------------------------------------------------------------------------------------

//ip and port submit button callback

static void enter_callback( GtkButton *button, GObject *context_object )
{
   gchar part1[100]; 
   gchar part2[100];
   gchar part3[50];
   gchar part4[50];

  g_stpcpy (part1, "alsasrc latency-time=20000 buffer-time=20000 ! audioconvert ! rtpL16pay ! udpsink host=");
  g_stpcpy (part2, ip );
  g_stpcpy (part3, " port=");
  g_stpcpy (part4, port	 );

  g_strlcat (part1, part2 , 150);
  g_strlcat (part1, part3 , 200);
  g_strlcat (part1, part4 , 250);
   
    pl2 = part1;
    pipeline2 = gst_parse_launch(pl2,NULL);
    gst_element_set_state (pipeline2, GST_STATE_PLAYING);
    	  g_print("%s  ",pl2);

    GtkLabel *accept_lable1 = g_object_get_data (context_object, "label6");
    GtkLabel *accept_lable2 = g_object_get_data (context_object, "label7");
    GtkEntry *accept_entry1 = g_object_get_data (context_object, "entry1");
    GtkEntry *accept_entry2 = g_object_get_data (context_object, "entry2");
    ip = gtk_entry_get_text (accept_entry1);
    port = gtk_entry_get_text (accept_entry2);

  //ektipwsi sto gui-------------------------------------
    const char *entry1_in = gtk_entry_get_text (accept_entry1);
    gtk_label_set_text (accept_lable1, entry1_in);
    const char *entry2_in = gtk_entry_get_text (accept_entry2);
    gtk_label_set_text (accept_lable2, entry2_in);
 //-------------------------------------------------------------
  
}



static void play_cb1 () {
  gst_element_set_state (pipeline1, GST_STATE_PLAYING);
}
  
/* This function is called when the PAUSE button is clicked */
static void pause_cb1 () {
  gst_element_set_state (pipeline1, GST_STATE_PAUSED);
}

/* This function is called when the STOP button is clicked */
static void stop_cb1 () {
  gst_element_set_state (pipeline1, GST_STATE_READY);
}
  

/* This function is called when the PLAY2 button is clicked */
static void play_cb2 () {
  gst_element_set_state (pipeline2, GST_STATE_PLAYING);
}
  
/* This function is called when the PAUSE2 button is clicked */
static void pause_cb2 () {
  gst_element_set_state (pipeline2, GST_STATE_PAUSED);
}     	


  
/* This function is called when the STOP2 button is clicked */
static void stop_cb2 () {
  gst_element_set_state (pipeline2, GST_STATE_READY);
}
  


/* This function is called when the PLAY3 button is clicked */
static void play_cb3 () {
  gst_element_set_state (pipeline3, GST_STATE_PLAYING);
}
  
/* This function is called when tgcc pipeline.c -o pipeline `pkg-config --cflags --libs gtk+-2.0 gstreamer-interfaces-0.10 gstreamer-1.0`he PAUSE3 button is clicked */
static void pause_cb3 () {
  gst_element_set_state (pipeline3, GST_STATE_PAUSED);
}

  
/* This function is called when the STOP3 button is clicked */
static void stop_cb3 () {
  gst_element_set_state (pipeline3, GST_STATE_READY);
}

static void
print_hello (GtkWidget *button,
             GObject *context_object)
{
  g_print ("Hello World\n");
}



//-------------------------------------------------------------------------------------------------------------




void buttons(){
    


  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "builder-glade.ui", &error) == 0)
    {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      //	return 1;
    }

  /* Connect signal handlers to the constructed widgets. */
  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

  label6 = gtk_builder_get_object (builder, "label6");
  label7 = gtk_builder_get_object (builder, "label7");
  entry1 = gtk_builder_get_object (builder, "entry1");
  entry2 = gtk_builder_get_object (builder, "entry2");

   
  submit = gtk_builder_get_object (builder, "submit");  
    g_object_set_data(G_OBJECT(submit), "label6", label6);
    g_object_set_data(G_OBJECT(submit), "label7", label7);
    g_object_set_data(G_OBJECT(submit), "entry1", entry1);
    g_object_set_data(G_OBJECT(submit), "entry2", entry2);

  g_signal_connect (GTK_BUTTON(submit), "clicked", G_CALLBACK (enter_callback), submit);  



  

  button = gtk_builder_get_object (builder, "play1");
  g_signal_connect (button, "clicked", G_CALLBACK (play_cb1), NULL);

  button = gtk_builder_get_object (builder, "pause1");
  g_signal_connect (button, "clicked", G_CALLBACK (pause_cb1), NULL);

  button = gtk_builder_get_object (builder, "stop1");
  g_signal_connect (button, "clicked", G_CALLBACK (stop_cb1), NULL);


  
  button = gtk_builder_get_object (builder, "play2");
  g_signal_connect (button, "clicked", G_CALLBACK (play_cb2), NULL);

  button = gtk_builder_get_object (builder, "pause2");
  g_signal_connect (button, "clicked", G_CALLBACK (pause_cb2), NULL);

  button = gtk_builder_get_object (builder, "stop2");
  g_signal_connect (button, "clicked", G_CALLBACK (stop_cb2), NULL);



  button = gtk_builder_get_object (builder, "play3");
  g_signal_connect (button, "clicked", G_CALLBACK (play_cb3), NULL);

  button = gtk_builder_get_object (builder, "pause3");
  g_signal_connect (button, "clicked", G_CALLBACK (pause_cb3), NULL);

  button = gtk_builder_get_object (builder, "stop3");
  g_signal_connect (button, "clicked", G_CALLBACK (stop_cb3), NULL);
  





}


//----------------------------------------------------------------------------------------------------


int main (int   argc,   char *argv[])
{


  /* Initialize GTK */
  gtk_init (&argc, &argv);
  /* Initialize GStreamer */
  gst_init (&argc, &argv);
  
  

  
  

  /*build the pipeline1 direct sound*/
  	pipeline1 = gst_parse_launch(pl1,NULL);





  /*build the pipeline2 to server*/
	pipeline2 = gst_parse_launch(pl2,NULL);
 


  /*build the pipeline3 from server*/
 
	pipeline3 = gst_parse_launch(pl3,NULL);


  buttons();
  
  gtk_main ();




  return 0;
}


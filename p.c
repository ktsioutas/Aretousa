/*EDW TA PIPELINES FTIAXNONTAI ME TON DYSKOLO TROPO*/
/*gcc pipeline.c -o pipeline `pkg-config --cflags --libs gtk+-2.0 gstreamer-interfaces-0.10 gstreamer-1.0`
*/

#include <gst/gst.h>
#include <string.h>
  
#include <gtk/gtk.h>
#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>
  
#include <gdk/gdk.h>
#if defined (GDK_WINDOWING_X11)
#include <gdk/gdkx.h>
#elif defined (GDK_WINDOWING_WIN32)
#include <gdk/gdkwin32.h>
#elif defined (GDK_WINDOWING_QUARTZ)
#include <gdk/gdkquartz.h>
#endif



  
/* Structure to contain all our information, so we can pass it around */
typedef struct _CustomData {
  GstElement *pipeline, *pipeline2, *pipeline3, *pipeline4,*pipeline5,*pipeline6,*pipeline7,*pipeline8,*pipeline9;           /* pipelines initialisation */

  GstElement 	    *audio_source , *audio_sink , *visual ,*tee , *audio_queue, *video_queue , *video_convert, *video_sink,
	     *audio_udp , *audio_convert, *audio_rtp,
		*audio_source2 , *audio_sink2 , *visual2 ,*tee2 , *audio_queue2, *video_queue2 , *video_convert2, *video_sink2,
	     *udp2 , *audio_convert2, *rtp2,
		*audio_source3 , *audio_sink3 , *visual3 ,*tee3 , *audio_queue3, *video_queue3 , *video_convert3, *video_sink3,
	     *udp3 , *audio_convert3, *rtp3;

 
  GstState state;                 /* Current state of the pipeline */
  gint64 duration;                /* Duration of the clip, in nanoseconds */
} CustomData;







/* This function is called when the GUI toolkit creates the physical window that will hold the video.
 * At this point we can retrieve its handler (which has a different meaning depending on the windowing system)
 * and pass it to GStreamer through the XOverlay interface. */
static void realize_cb (GtkWidget *widget, CustomData *data) {
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


  /* Pass it to playbin2, which implements XOverlay and will forward it to the video sink */
  gst_x_overlay_set_window_handle (GST_X_OVERLAY (data->pipeline), window_handle);
}




/* This function is called when the PLAY button is clicked */
static void play_cb (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline, GST_STATE_PLAYING);
}
  
/* This function is called when the PAUSE button is clicked */
static void pause_cb (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline, GST_STATE_PAUSED);
}



  
/* This function is called when the STOP button is clicked */
static void stop_cb (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline, GST_STATE_READY);
}
  





/* This function is called when the PLAY2 button is clicked */
static void play_cb2 (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline2, GST_STATE_PLAYING);
}
  
/* This function is called when the PAUSE2 button is clicked */
static void pause_cb2 (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline2, GST_STATE_PAUSED);
}
  
/* This function is called when the STOP2 button is clicked */
static void stop_cb2 (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline2, GST_STATE_READY);
}
  


/* This function is called when the PLAY3 button is clicked */
static void play_cb3 (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline3, GST_STATE_PLAYING);
}
  
/* This function is called when the PAUSE3 button is clicked */
static void pause_cb3 (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline3, GST_STATE_PAUSED);
}
  
/* This function is called when the STOP3 button is clicked */
static void stop_cb3 (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline3, GST_STATE_READY);
}


/* This function is called when the PLAY4 button is clicked */
static void play_cb4 (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline4, GST_STATE_PLAYING);
}
  
/* This function is called when the PAUSE4 button is clicked */
static void pause_cb4 (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline4, GST_STATE_PAUSED);
}
  
/* This function is called when the STOP4 button is clicked */
static void stop_cb4 (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline4, GST_STATE_READY);
}


/* This function is called when the PLAY5 button is clicked */
static void play_cb5 (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline5, GST_STATE_PLAYING);
}
  
/* This function is called when the PAUSE5 button is clicked */
static void pause_cb5 (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline5, GST_STATE_PAUSED);
}
  
/* This function is called when the STOP5 button is clicked */
static void stop_cb5 (GtkButton *button, CustomData *data) {
  gst_element_set_state (data->pipeline5, GST_STATE_READY);
}



/* This function is called when the main window is closed */
static void delete_event_cb (GtkWidget *widget, GdkEvent *event, CustomData *data) {
  stop_cb (NULL, data);
  gtk_main_quit ();
}

/*This function is for the entry text*/
static void enter_callback( GtkWidget *widget,
                            GtkWidget *ip, GtkWidget *port)
{
  const gchar *server_ip;
  const gchar *udp_port;
  server_ip = gtk_entry_get_text (GTK_ENTRY (ip));
  udp_port = gtk_entry_get_text (GTK_ENTRY (port));
  printf ("Server IP: %s\n", server_ip);
  printf ("UDP Port: %s\n", udp_port);
  
  
}




/* This creates all the GTK+ widgets that compose our application, and registers the callbacks */
static void create_ui (CustomData *data) {
  GtkWidget *main_window;  /* The uppermost window, containing all other windows */

  GtkWidget *main_box;     /* VBox to hold main_hbox and the controls */
  GtkWidget *main_hbox;    /* HBox to hold the text entries */
  GtkWidget *main_hbox2;   /* HBox to hold the text entries */
  GtkWidget *main_hbox3;   /* HBox to hold the text entries */
  GtkWidget *controls, *controls2, *controls3,*controls4,*controls5,*controls6,*controls7,*controls8;     /* HBox to hold the buttons */
  GtkWidget *play_button, *pause_button, *stop_button; /* Buttons */
  GtkWidget *play_button2, *pause_button2, *stop_button2; /* Buttons 2*/
  GtkWidget *play_button3, *pause_button3, *stop_button3; /* Buttons 3*/
  GtkWidget *play_button4, *pause_button4, *stop_button4; /* Buttons 4*/
  GtkWidget *play_button5, *pause_button5, *stop_button5; /* Buttons 5*/
  GtkWidget *submit_button;
  GtkWidget *vbox, *hbox;  
  GtkWidget *label, *label2, *label3, *label4, *label5, *label6,*label7,*label8,*label9;
  GtkWidget *entry1, *entry2, *entry3;  
  gint tmp_pos;
  gchar *ip = NULL;
  main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (G_OBJECT (main_window), "delete-event", G_CALLBACK (delete_event_cb), data);

    /*Text input field #1*/
  entry1 = gtk_entry_new ();  
  g_signal_connect (entry1, "activate", G_CALLBACK (enter_callback), entry1);


    /*Text input field #2*/
  entry2 = gtk_entry_new ();  
  g_signal_connect (entry2, "activate",  G_CALLBACK(enter_callback), entry2);
  gtk_editable_select_region (GTK_EDITABLE (entry2), 0, GTK_ENTRY (entry2)->text_length);

  /*submit_button*/
  submit_button = gtk_button_new_with_label("Submit");
  g_signal_connect (G_OBJECT(submit_button), "clicked", G_CALLBACK (enter_callback), entry1);

  /*buttons 1*/
  play_button = gtk_button_new_from_stock (GTK_STOCK_MEDIA_PLAY);
  g_signal_connect (G_OBJECT (play_button), "clicked", G_CALLBACK (play_cb), data);
  
  pause_button = gtk_button_new_from_stock (GTK_STOCK_MEDIA_PAUSE);
  g_signal_connect (G_OBJECT (pause_button), "clicked", G_CALLBACK (pause_cb), data);
  
  stop_button = gtk_button_new_from_stock (GTK_STOCK_MEDIA_STOP);
  g_signal_connect (G_OBJECT (stop_button), "clicked", G_CALLBACK (stop_cb), data);
  

    /*buttons 2*/
  play_button2 = gtk_button_new_from_stock (GTK_STOCK_MEDIA_PLAY);
  g_signal_connect (G_OBJECT (play_button2), "clicked", G_CALLBACK (play_cb2), data);
  
  pause_button2 = gtk_button_new_from_stock (GTK_STOCK_MEDIA_PAUSE);
  g_signal_connect (G_OBJECT (pause_button2), "clicked", G_CALLBACK (pause_cb2), data);
  
  stop_button2 = gtk_button_new_from_stock (GTK_STOCK_MEDIA_STOP);
  g_signal_connect (G_OBJECT (stop_button2), "clicked", G_CALLBACK (stop_cb2), data);


    /*buttons 3*/
  play_button3 = gtk_button_new_from_stock (GTK_STOCK_MEDIA_PLAY);
  g_signal_connect (G_OBJECT (play_button3), "clicked", G_CALLBACK (play_cb3), data);
  
  pause_button3 = gtk_button_new_from_stock (GTK_STOCK_MEDIA_PAUSE);
  g_signal_connect (G_OBJECT (pause_button3), "clicked", G_CALLBACK (pause_cb3), data);
  
  stop_button3 = gtk_button_new_from_stock (GTK_STOCK_MEDIA_STOP);
  g_signal_connect (G_OBJECT (stop_button3), "clicked", G_CALLBACK (stop_cb3), data);

 
    /*buttons 4*/
  play_button4 = gtk_button_new_from_stock (GTK_STOCK_MEDIA_PLAY);
  g_signal_connect (G_OBJECT (play_button3), "clicked", G_CALLBACK (play_cb4), data);
  
  pause_button4 = gtk_button_new_from_stock (GTK_STOCK_MEDIA_PAUSE);
  g_signal_connect (G_OBJECT (pause_button3), "clicked", G_CALLBACK (pause_cb4), data);
  
  stop_button4 = gtk_button_new_from_stock (GTK_STOCK_MEDIA_STOP);
  g_signal_connect (G_OBJECT (stop_button3), "clicked", G_CALLBACK (stop_cb4), data);

 
    

  

  /*hbox to hold text entry SERVER IP*/
  label = gtk_label_new ("Server  IP");
  main_hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (main_hbox), label, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (main_hbox), entry1, FALSE, FALSE, 2);  
	  
  /*hbox to hold udp port number*/
  label2 = gtk_label_new ("UDP PORT");
  main_hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (main_hbox2), label2, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (main_hbox2), entry2, FALSE, FALSE, 2);  



  controls = gtk_hbox_new (FALSE, 0);
  label4 = gtk_label_new ("Your Sound");
  gtk_box_pack_start (GTK_BOX (controls), label4, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls), play_button, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls), pause_button, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls), stop_button, FALSE, FALSE, 2);


  controls2 = gtk_hbox_new (FALSE, 0);
  label5 = gtk_label_new ("To Server");
  gtk_box_pack_start (GTK_BOX (controls2), label5, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls2), play_button2, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls2), pause_button2, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls2), stop_button2, FALSE, FALSE, 2);


  controls3 = gtk_hbox_new (FALSE, 0);
  label6 = gtk_label_new ("From Server");
  gtk_box_pack_start (GTK_BOX (controls3), label6, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls3), play_button3, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls3), pause_button3, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls3), stop_button3, FALSE, FALSE, 2);


  controls7 = gtk_hbox_new (FALSE, 0);
  label6 = gtk_label_new ("From Server");
  gtk_box_pack_start (GTK_BOX (controls7), label6, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls7), play_button4, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls7), pause_button4, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls7), stop_button4, FALSE, FALSE, 2);



  controls4 = gtk_hbox_new (FALSE, 0);
  label7 = gtk_label_new ("Server IP:");
  label9 = gtk_label_new ("Server IP:");
  label9 = entry1;
  gtk_box_pack_start (GTK_BOX (controls4), label7, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (controls4), label9, FALSE, FALSE, 2);



  controls5 = gtk_hbox_new (FALSE, 0);
  label8 = gtk_label_new ("UDP Port:");
  gtk_box_pack_start (GTK_BOX (controls5), label8, FALSE, FALSE, 2);

  /*hbox to hold submit_button*/
  controls6 = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (controls6), submit_button, FALSE, FALSE, 2);


  main_box = gtk_vbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (main_box), main_hbox, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (main_box), main_hbox2, FALSE, FALSE, 2);
 // gtk_box_pack_start (GTK_BOX (main_box), main_hbox3, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (main_box), controls, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (main_box), controls2, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (main_box), controls3, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (main_box), controls4, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (main_box), controls5, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (main_box), controls6, FALSE, FALSE, 2);
  gtk_box_pack_start (GTK_BOX (main_box), controls7, FALSE, FALSE, 2);

  gtk_container_add (GTK_CONTAINER (main_window), main_box);
  gtk_window_set_default_size (GTK_WINDOW (main_window), 400, 400);  //WINDOW SIZE 640 X 480
   gtk_window_set_title(GTK_WINDOW (main_window),"SoLAN"); //Set window title

  






  gtk_widget_show_all (main_window);
}

/* This function is called periodically to refresh the GUI */
static gboolean refresh_ui (CustomData *data) {
  GstFormat fmt = GST_FORMAT_TIME;
  gint64 current = -1;
  
  /* We do not want to update anything unless we are in the PAUSED or PLAYING states */
  if (data->state < GST_STATE_PAUSED)
    return TRUE;

}







  
int main(int argc, char *argv[]) {

     	
  const gchar *pipeline1 = "alsasrc buffer-time=15000 ! alsasink buffer-time=15000 "; 
  const gchar *pipeline2 = "alsasrc latency-time=2000 buffer-time=2000 ! audioconvert ! rtpL16pay ! udpsink host=83.212.101.51 port=1000"; 
  const gchar *pipeline3 = "udpsrc port=7151 ! application/x-rtp,media=audio, clock-rate=88200, encoding-name=L16, payload=96 ! rtpL16depay ! audioconvert ! alsasink latency-time=2000  buffer-time=2000 ";
  

  const gchar *pipeline4 = "alsasrc buffer-time=20000 ! wavescope ! ffmpegcolorspace ! autovideosink ";
  const gchar *pipeline5 = "alsasrc buffer-time=20000 ! wavescope ! ffmpegcolorspace ! autovideosink ";
 
  CustomData data;
  GError *error;
  GstElement *pipeline,*pipeline2,*pipeline3,*pipeline4,*pipeline5,*pipeline6,*pipeline7;
  GstBus *bus;
  GstMessage *msg;
  GstPadTemplate *tee_src_pad_template;
  GstPad *tee_audio_pad, *tee_video_pad;
  GstPad *queue_audio_pad, *queue_video_pad;
  gchar *string = NULL;
  
  


  /* Initialize GTK */
  gtk_init (&argc, &argv);  

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Initialize our data structure */
  memset (&data, 0, sizeof (data));
  data.duration = GST_CLOCK_TIME_NONE;
  
  

  /*build the pipeline1*/
  	data.pipeline = gst_parse_launch(pipeline1,NULL);

/*("alsasrc buffer-time=20000 ! tee name=tee .src%d! queue ! alsasink buffer-time=20000 tee.src%d ! queue ! wavescope ! ffmpegcolorspace ! autovideosink tee.src%d ! queue ! audioconvert ! rtpL16pay ! udpsink host=10.42.0.1 port=1000 -v",NULL);
*/
 



  /*build the pipeline2*/
	data.pipeline2 = gst_parse_launch(pipeline2,NULL);
 


  /*build the pipeline3*/
 
	data.pipeline3 = gst_parse_launch(pipeline3,NULL);



/*"udpsrc port=3000 ! application/x-rtp,media=(string)audio,clock-rate=(int)88200, encoding-name=(string)L16,payload=(int)96 ! rtpL16depay ! audioconvert ! wavescope ! ffmpegcolorspace ! autovideosink "*/

/*
("udpsrc port=3000 ! application/x-rtp,media=(string)audio,clock-rate=(int)88200, encoding-name=(string)L16,payload=(int)96 ! rtpL16depay ! audioconvert ! wavescope ! ffmpegcolorspace ! autovideosink -v",&error); 
 
*/ 













  




  /* Create the GUI */ 
  create_ui (&data);

  /* Start the GTK main loop. We will not regain control until gtk_main_quit is called. */
  gtk_main ();   




  /* Wait until error or EOS */
  bus = gst_element_get_bus (data.pipeline);
  msg = gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);
  



  /* Register a function that GLib will call every second */
  g_timeout_add_seconds (1, (GSourceFunc)refresh_ui, &data);


 

 /* Free resources */
  if (msg != NULL)
    gst_message_unref (msg);
  gst_object_unref (bus);
  gst_element_set_state (data.pipeline, GST_STATE_NULL);
  gst_object_unref (data.pipeline);


  return 0;
}

#include <gtk/gtk.h>
//p2p audio application based on gstreamer and gtk
/*EDW TA PIPELINES FTIAXNONTAI ME TON eukolo TROPO*/
/*gcc p2p-stream-caps.c -o p2p-stream-caps `pkg-config --cflags --libs gtk+-3.0 gstreamer-interfaces-0.10 gstreamer-0.10`
*/

#include <gst/gst.h>
#include <string.h>
#include <math.h>
#include <gtk/gtk.h>
#include <gst/gst.h>
//#include <gst/interfaces/xoverlay.h>
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

#ifndef M_LN10
#define M_LN10 (log(10.0))
#endif

typedef struct _CustomData {
  GstElement *pipeline1, *pipeline2, *pipeline3, *pipeline4,*pipeline5,*pipeline6,*pipeline7,*pipeline8,*pipeline9, *pipeline10;           /* pipelines initialisation */

  GstElement 	    *audio_source1 , *audio_sink1 , *visual ,*tee1 , *audio_queue11,*audio_queue12, *video_queue , *video_convert, *video_sink,
	     *audio_udp1 , *audio_convert1, *audio_rtp1, *audio_volume1, *audio_opusenc1, *audio_opusdec1, *factory,*audio_rtpopusenc1, *audio_wavenc1, *audio_filesink1, *audioresample1,

		*audio_source2 , *audio_sink2 , *visual2 ,*tee2 , *audio_queue21,*audio_queue22, *video_queue2 , *video_convert2, *video_sink2,
	     *audio_udp2 , *audio_convert2, *audio_rtp2, *audio_volume2, *audio_opusenc2, *audio_rtpopuspay2, *audio_wavenc2, *audio_filesink2, *audioresample2,

		*audio_source3 , *audio_sink3 , *visual3 ,*tee3 , *audio_queue31,*audio_queue32, *video_queue3 , *video_convert3, *video_sink3,
	     *audio_udp3 , *audio_convert3, *audio_rtp3 , *audio_volume3, *audio_opusdec3, *audio_rtpopusenc3, *audio_rtpopusdepay3,*audio_wavenc3, *audio_filesink3, *audioresample3,

		*audio_source4 , *audio_sink4 , *visual4 ,*tee4 , *audio_queue41,*audio_queue42, *video_queue4 , *video_convert4, *video_sink4,*audio_wavenc4, *audio_filesink4,
	     *audio_udp4 , *audio_convert4, *audio_rtp4 , *audio_volume4, *audio_opusenc4, *audio_opusdec4, *audio_rtpopusenc4, *audio_rtpopusdepay4,

		*audio_source5 , *audio_sink5 , *visual5 ,*tee5 , *audio_queue5, *video_queue5 , *video_convert5, *video_sink5,
	     *audio_udp5 , *audio_convert5, *audio_rtp5 , *audio_volume5, *audio_opusenc5,*audio_opusdec5, *audio_rtpopusenc5, *audio_rtpopuspay5,*audio_rtpopusdepay5,

		*audio_source6 , *audio_sink6 , *visual6 ,*tee6 , *audio_queue61,*audio_queue62, *video_queue6 , *video_convert6, *video_sink6,
	     *audio_udp6 , *audio_convert6, *audio_rtp6 , *audio_volume6, *audio_opusdec6, *audio_rtpopusenc6, *audio_rtpopusdepay6,*audio_wavenc6, *audio_filesink6, *audioresample6,


		*audio_source8 , *audio_sink8 , *visual8 ,*tee8 , *audio_queue8, *video_queue8 , *video_convert8, *video_sink8,*audio_wavenc8, *audio_filesink8,
	     *audio_udp8 , *audio_convert8, *audio_rtp8 , *audio_volume8, *audio_opusdec8,*audio_opusenc8, *audio_rtpopusenc8, *audio_rtpopusdepay8,


		*audio_source7 , *audio_sink7 , *visual7 ,*tee7 , *audio_queue71, *audio_queue72 , *video_convert7, *video_sink7, *audio_interleave7,
	     *audio_udp7 , *audio_convert7, *audio_rtp7 , *audio_volume7, *audio_opusdec7, *audio_rtpopusenc7, *audio_rtpopusdepay7,*audio_wavenc7, *audio_filesink7, *audioresample7,

		*audio_source9 , *audio_sink9 , *visual9 ,*tee9 , *audio_queue91, *audio_queue92 , *video_convert9, *video_sink9,
	     *audio_udp9 , *audio_convert9, *audio_rtp9 , *audio_volume9, *audio_opusdec9, *audio_rtpopusenc9, *audio_rtpopusdepay9,*audio_wavenc9, *audio_filesink9, *audioresample9,


		*audio_source10 , *audio_sink10 , *visual10 ,*tee10 , *audio_queue10, *video_queue10 , *video_convert10, *video_sink10,
	     *audio_udp10 , *audio_convert10, *audio_rtp10 , *audio_volume10, *audio_opusdec10, *audio_opusenc10, *audio_rtpopusenc10, *audio_rtpopusdepay10,*audio_wavenc10, *audio_filesink10, 		*audioresample10;
             
  GstPad *audio_pad1;
  GstStructure *structure;
  GstCaps *alsa_caps;
  GstCaps *caps, *caps1 ;      		  /*Caps*/
  GstState state;                 /* Current state of the pipeline */

} CustomData;

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

  GObject *volume1;
  GObject *volume2;
  GObject *volume3;
  GObject *volume4;
  GObject *samp_rate;
  GError *error = NULL;
  GtkBuilder *builder;
  GObject *window;
  GObject *button, *change_buf, *pac_sz;
  GObject *submit;
  GObject *label6;
  GObject *label7;
  GObject *entry1;
  GObject *entry2;
  GObject *entry3;
  GObject *entry4;
  GObject *entry_text1;
  GObject *entry_text2;
  GtkWidget *main_hbox; 
  GtkEntry *ip_entry;
  GtkEntry *port_entry;
  const gchar *ip;
  gint port;
  gint64 s_r;
  gint opus,pcm, format;
 
  GObject *context_object;
  GObject *context_object_buf;
  CustomData data;



//------------------------------------------------------------------PPPPPPPPPPPPPPPPPPPPPPPCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCMMMMMMMMMMMMMMMMMMMMMMMMMMM------------------------------




static void pipelines_pcm()
{

  g_printf("You are using PCM format\n");
//-------------------------BUILD THE PIPELINE1------START---------LISTEN ΜΥ SOUND------------------------------------

//                                
//-------ALSASRC-----------VOLUME---------AUDIOCONVERT---------ALSASINK(LISTEN ΜΥ SOUND)

  data.audioresample1 = gst_element_factory_make("audioresample","audioresample1"); 	
  data.tee1 = gst_element_factory_make("tee", "tee1");


  data.audio_source1 = gst_element_factory_make("alsasrc", "audio_source1");
  //g_object_set (data.audio_source1 , "device", "hw:0", NULL);
  data.audio_volume1 = gst_element_factory_make("volume", "audio_volume1");
  data.audio_convert1 = gst_element_factory_make("audioconvert", "audio_convert1");
  data.audio_sink1 = gst_element_factory_make("alsasink", "audio_sink1");
  //g_object_set (data.audio_sink1 , "device", "hw:0", NULL);
  data.pipeline1 = gst_pipeline_new ("pipeline1");
  
  if (!data.pipeline1 || !data.audio_source1  || !data.audio_volume1 || !data.audio_convert1 || !data.audio_sink1) {
    g_printerr ("Not all pl1 elements could be created.\n");
  }
  gst_bin_add_many (GST_BIN (data.pipeline1),data.audio_source1,  data.audio_volume1,data.audio_convert1, data.audio_sink1,  NULL);  		    

  if (!gst_element_link_many (data.audio_source1, data.audio_volume1,data.audio_convert1, data.audio_sink1,NULL))  
  {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (data.pipeline1);

  }

  





//----------------------------------------------------------------------------------------------------------------------



//-------------------------BUILD THE PIPELINE4------START---------ΜΥ SOUND-RECORDED-----------------------------------
  
//-----------------------------ALSASRC--------AUDIOCONVERT STEREO TO MONO----------WAVENC---------FILESINK(WRITE TO FILE)
  data.alsa_caps = gst_caps_from_string("audio/x-raw-int, endianness=1234, signed=true, width=16, depth=16, rate=88200, channels=1");
  data.audio_source4 = gst_element_factory_make("alsasrc", "audio_source4");
  g_object_set (data.audio_source4 , "caps", data.alsa_caps, NULL);

  //data.audio_interleave4 = gst_element_factory_make("interleave", "audio_interleave4");

  //g_object_set (data.audio_source4 , "device", "hw:0", NULL);
  data.audio_wavenc4 = gst_element_factory_make("wavenc", "audio_wavenc4");

  //gst_object_unref(data.alsa_caps);
  //gst_caps_unref(data.alsa_caps);
  data.audio_filesink4 = gst_element_factory_make("filesink", "audio_filesink4");
  g_object_set (data.audio_filesink4 , "location", "out_pcm.wav", NULL);
  data.pipeline4 = gst_pipeline_new ("pipeline4");

  if (!data.pipeline4 || !data.audio_source4  || !data.audio_wavenc4 || !data.audio_filesink4  ) {
    g_printerr ("Not all pl4 elements could be created.\n");
  }
  gst_bin_add_many (GST_BIN (data.pipeline4),data.audio_source4,   data.audio_wavenc4,data.audio_filesink4,  NULL);  		    

  if (!gst_element_link_many (data.audio_source4, data.audio_wavenc4,data.audio_filesink4,NULL))  
  {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (data.pipeline4);

  }










//-------------------------BUILD THE PIPELINE2------START-----------MY STREAM TO SERVER----------------------------------

//                                
//-------ALSASRC---------AUDIOCONVERT-----RTPL16PAY-------UDPSINK(SEND TO SERVER)
//				  

  data.audio_source2 = gst_element_factory_make("alsasrc", "audio_source2");
  g_object_set (data.audio_source2 , "caps", data.alsa_caps, NULL);

  //g_object_set (data.audio_source2 , "device", "hw:0", NULL);
  data.audio_convert2 = gst_element_factory_make("audioconvert", "audio_convert2");
  data.audio_rtp2 = gst_element_factory_make("rtpL16pay", "audio_rtp2");
  //g_object_set (data.audio_rtp2 , "mtu", 90 , NULL);
  data.audio_sink2 = gst_element_factory_make("alsasink", "audio_sink2");
  //g_object_set (data.audio_sink2 , "device", "hw:0", NULL);
  data.audio_udp2 = gst_element_factory_make("udpsink", "audio_udp2");

  data.pipeline2 = gst_pipeline_new ("pipeline2");
  if (!data.pipeline2 || !data.audio_source2 || !data.audio_convert2 || !data.audio_rtp2 || !data.audio_udp2  ) {
    g_printerr ("Not all pl2 elements could be created.\n");
    return ;
  }
  gst_bin_add_many (GST_BIN (data.pipeline2), data.audio_source2, data.audio_convert2, data.audio_rtp2, data.audio_udp2, NULL);
  if (!gst_element_link_many (data.audio_source2, data.audio_convert2, data.audio_rtp2, data.audio_udp2,NULL)) {
    g_printerr ("Elements  pl2 could not be linked.\n");
    gst_object_unref (data.pipeline2);
    return ;
  }









//-------------------------BUILD THE PIPELINE3------START----------MY STREAM FROM SERVER----------LISTEN---AND---RECORD-------------------------

//                                                           QUEUE------VOLUME-----ALSASINK(LISTEN MY SOUND)
//-------UDPSRC------RTPL16DEPAY---AUDIOCONVERT----TEE------|
//				                             QUEUE------WAVENC-----FILESINK(RECORD MY SOUND WRITE TO FILE)

  data.caps = gst_caps_from_string("application/x-rtp,media=audio, clock-rate=88200, encoding-name=L16, payload=96");
  data.audio_source3 = gst_element_factory_make("udpsrc", "audio_source3");
  g_object_set (data.audio_source3 , "port",7150 ,"caps", data.caps , NULL);
  gst_caps_unref(data.caps);
  data.tee3 = gst_element_factory_make("tee","tee3");
  data.audio_wavenc3 = gst_element_factory_make("wavenc","audio_wavenc3");
  data.audio_filesink3 = gst_element_factory_make("filesink","audio_filesink3");
  g_object_set (data.audio_filesink3 , "location", "in_pcm.wav", NULL);
  data.audio_queue31 = gst_element_factory_make("queue", "audio_queue31");
  data.audio_queue32 = gst_element_factory_make("queue", "audio_queue32");
  data.audio_volume3 = gst_element_factory_make("volume", "audio_volume3");
  data.audio_convert3 = gst_element_factory_make("audioconvert", "audio_convert3");
  data.audio_rtp3 = gst_element_factory_make("rtpL16depay", "audio_rtp3");
  data.audio_sink3 = gst_element_factory_make("alsasink", "audio_sink3");
  //g_object_set (data.audio_sink3 , "device", "hw:0", NULL);
  data.pipeline3 = gst_pipeline_new ("pipeline3");
  if (!data.pipeline3 || !data.audio_source3 || !data.audio_rtp3 || !data.audio_convert3 || !data.audio_sink3 || !data.tee3 || !data.audio_queue31 || !data.audio_queue32 || !data.audio_wavenc3              || !data.audio_filesink3 ) {
    g_printerr ("Not all elements pl3 could be created.\n");
    return ;
  }

  gst_bin_add_many (GST_BIN (data.pipeline3), data.audio_source3, data.audio_rtp3, data.audio_convert3, data.tee3, data.audio_queue31, data.audio_sink3, data.audio_volume3, NULL);
  gst_bin_add_many (GST_BIN (data.pipeline3), data.audio_queue32, data.audio_wavenc3, data.audio_filesink3, NULL);
  
 
  if (!gst_element_link_many (data.audio_source3, data.audio_rtp3,  data.audio_convert3, data.tee3, data.audio_queue31, data.audio_volume3,  data.audio_sink3,NULL) 
      || !gst_element_link_many (data.tee3, data.audio_queue32, data.audio_wavenc3, data.audio_filesink3, NULL) ) {
    g_printerr ("Elements in pl3 could not be linked.\n");
    gst_object_unref (data.pipeline3);
    return ;
  }

  




//-------------------------BUILD THE PIPELINE5------START----------MY STREAM FROM SERVER----------LISTEN----------------------------

//                                                           
//-------UDPSRC------RTPL16DEPAY---AUDIOCONVERT-----VOLUME-----ALSASINK(LISTEN MY SOUND FROM SERVER)
//				                             

  data.caps = gst_caps_from_string("application/x-rtp,media=audio, clock-rate=88200, encoding-name=L16, payload=96");
  data.audio_source5 = gst_element_factory_make("udpsrc", "audio_source5");
  g_object_set (data.audio_source9 , "port",7150 ,"caps", data.caps , NULL);
  gst_caps_unref(data.caps);
  data.audio_volume5 = gst_element_factory_make("volume", "audio_volume5");
  data.audio_convert5 = gst_element_factory_make("audioconvert", "audio_convert5");
  data.audio_rtp5 = gst_element_factory_make("rtpL16depay", "audio_rtp5");
  data.audio_sink5 = gst_element_factory_make("alsasink", "audio_sink5");
  //g_object_set (data.audio_sink9 , "device", "hw:0", NULL);
  data.pipeline5 = gst_pipeline_new ("pipeline5");
  if (!data.pipeline5 || !data.audio_source5 || !data.audio_rtp5 || !data.audio_convert5 || !data.audio_sink5 || !data.audio_volume5) {
    g_printerr ("Not all elements pl5 could be created.\n");
    return ;
  }

  gst_bin_add_many (GST_BIN (data.pipeline9), data.audio_source9, data.audio_rtp9, data.audio_convert9,  data.audio_volume9, data.audio_sink9, NULL);
  if (!gst_element_link_many (data.audio_source9, data.audio_rtp9,  data.audio_convert9, data.audio_volume9,  data.audio_sink9,NULL)) {
    g_printerr ("Elements in pl3 could not be linked.\n");
    gst_object_unref (data.pipeline9);
    return ;
  }

  



//-------------------------BUILD THE PIPELINE6------START----------CLIENT'S 2 STREAM FROM SERVER----------LISTEN----------------------------

//                                                           
//-------UDPSRC------RTPL16DEPAY---AUDIOCONVERT-----VOLUME-----ALSASINK
//

  data.caps = gst_caps_from_string("application/x-rtp,media=audio, clock-rate=88200, encoding-name=L16, payload=96");
  data.audio_source6 = gst_element_factory_make("udpsrc", "audio_source6");
  g_object_set (data.audio_source6 , "port",7150 ,"caps", data.caps , NULL);
  gst_caps_unref(data.caps);
  data.audio_volume6 = gst_element_factory_make("volume", "audio_volume6");
  data.audio_convert6 = gst_element_factory_make("audioconvert", "audio_convert6");
  data.audio_rtp6 = gst_element_factory_make("rtpL16depay", "audio_rtp6");
  data.audio_sink6 = gst_element_factory_make("alsasink", "audio_sink6");
  //g_object_set (data.audio_sink9 , "device", "hw:0", NULL);
  data.pipeline6 = gst_pipeline_new ("pipeline6");
  if (!data.pipeline6 || !data.audio_source6 || !data.audio_rtp6 || !data.audio_convert6 || !data.audio_sink6 || !data.audio_volume6) {
    g_printerr ("Not all elements pl6 could be created.\n");
    return ;
  }

  gst_bin_add_many (GST_BIN (data.pipeline6), data.audio_source6, data.audio_rtp6, data.audio_convert6,  data.audio_volume6, data.audio_sink6, NULL);
 
  if (!gst_element_link_many (data.audio_source6, data.audio_rtp6,  data.audio_convert6, data.audio_volume6,  data.audio_sink6,NULL)) {
    g_printerr ("Elements in pl6 could not be linked.\n");
    gst_object_unref (data.pipeline6);
    return ;
  }








//-------------------------BUILD THE PIPELINE7------START----------CLIENT'S 2 STREAM FROM SERVER----------LISTEN--AND---RECORD--------------------

//                                                           QUEUE------VOLUME-----ALSASINK
//-------UDPSRC------RTPL16DEPAY---AUDIOCONVERT----TEE------|
//				                             QUEUE------WAVENC-----FILESINK
  data.caps = gst_caps_from_string("application/x-rtp,media=audio, clock-rate=88200, encoding-name=L16, payload=96");
  data.audio_source7 = gst_element_factory_make("udpsrc", "audio_source7");
  g_object_set (data.audio_source7 , "port",7150 ,"caps", data.caps , NULL);
  gst_caps_unref(data.caps);
  data.tee7 = gst_element_factory_make("tee","tee7");
  data.audio_wavenc7 = gst_element_factory_make("wavenc","audio_wavenc7");
  data.audio_filesink7 = gst_element_factory_make("filesink","audio_filesink7");
  g_object_set (data.audio_filesink3 , "location", "client1_pcm.wav", NULL);
  data.audio_queue71 = gst_element_factory_make("queue", "audio_queue71");
  data.audio_queue72 = gst_element_factory_make("queue", "audio_queue72");
  data.audio_volume7 = gst_element_factory_make("volume", "audio_volume7");
  data.audio_convert7 = gst_element_factory_make("audioconvert", "audio_convert7");
  data.audio_rtp7 = gst_element_factory_make("rtpL16depay", "audio_rtp7");
  data.audio_sink7 = gst_element_factory_make("alsasink", "audio_sink7");
  //g_object_set (data.audio_sink3 , "device", "hw:0", NULL);
  data.pipeline7 = gst_pipeline_new ("pipeline3");
  if (!data.pipeline7 || !data.audio_source7 || !data.audio_rtp7 || !data.audio_convert7 || !data.audio_sink7 || !data.tee7 || !data.audio_queue71 || !data.audio_queue72 || !data.audio_wavenc7              || !data.audio_filesink7 ) {
    g_printerr ("Not all elements pl7 could be created.\n");
    return ;
  }

  gst_bin_add_many (GST_BIN (data.pipeline7), data.audio_source7, data.audio_rtp7, data.audio_convert7, data.tee7, data.audio_queue71, data.audio_sink7, data.audio_volume7, NULL);
  gst_bin_add_many (GST_BIN (data.pipeline7), data.audio_queue72, data.audio_wavenc7, data.audio_filesink7, NULL);
  
 
  if (!gst_element_link_many (data.audio_source7, data.audio_rtp7,  data.audio_convert7, data.tee7, data.audio_queue71, data.audio_volume7,  data.audio_sink7,NULL) 
      || !gst_element_link_many (data.tee7, data.audio_queue72, data.audio_wavenc7, data.audio_filesink7, NULL) ) {
    g_printerr ("Elements in pl7 could not be linked.\n");
    gst_object_unref (data.pipeline3);
    return ;
  }

  




//-------------------------BUILD THE PIPELINE8------START----------CLIENT'S 3 STREAM FROM SERVER----------LISTEN----------------------------

//                                                           
//-------UDPSRC------RTPL16DEPAY---AUDIOCONVERT-----VOLUME-----ALSASINK
//

  data.caps = gst_caps_from_string("application/x-rtp,media=audio, clock-rate=88200, encoding-name=L16, payload=96");
  data.audio_source8 = gst_element_factory_make("udpsrc", "audio_source8");
  g_object_set (data.audio_source6 , "port",7150 ,"caps", data.caps , NULL);
  gst_caps_unref(data.caps);
  data.audio_volume8 = gst_element_factory_make("volume", "audio_volume8");
  data.audio_convert8 = gst_element_factory_make("audioconvert", "audio_convert8");
  data.audio_rtp8 = gst_element_factory_make("rtpL16depay", "audio_rtp8");
  data.audio_sink8 = gst_element_factory_make("alsasink", "audio_sink8");
  //g_object_set (data.audio_sink9 , "device", "hw:0", NULL);
  data.pipeline8 = gst_pipeline_new ("pipeline8");
  if (!data.pipeline8 || !data.audio_source8 || !data.audio_rtp8 || !data.audio_convert8 || !data.audio_sink8 || !data.audio_volume8) {
    g_printerr ("Not all elements pl8 could be created.\n");
    return ;
  }

  gst_bin_add_many (GST_BIN (data.pipeline8), data.audio_source8, data.audio_rtp8, data.audio_convert8,  data.audio_volume8, data.audio_sink8, NULL);
 
  if (!gst_element_link_many (data.audio_source8, data.audio_rtp8,  data.audio_convert8, data.audio_volume8,  data.audio_sink8,NULL)) {
    g_printerr ("Elements in pl8 could not be linked.\n");
    gst_object_unref (data.pipeline6);
    return ;
  }






//-------------------------BUILD THE PIPELINE9------START----------CLIENT'S 3 STREAM FROM SERVER----------LISTEN--AND---RECORD--------------------

//                                                           QUEUE------VOLUME-----ALSASINK
//-------UDPSRC------RTPL16DEPAY---AUDIOCONVERT----TEE------|
//				                             QUEUE------WAVENC-----FILESINK



  data.caps = gst_caps_from_string("application/x-rtp,media=audio, clock-rate=88200, encoding-name=L16, payload=96");
  data.audio_source9 = gst_element_factory_make("udpsrc", "audio_source9");
  g_object_set (data.audio_source9 , "port",7150 ,"caps", data.caps , NULL);
  gst_caps_unref(data.caps);
  data.tee9 = gst_element_factory_make("tee","tee9");
  data.audio_wavenc9 = gst_element_factory_make("wavenc","audio_wavenc9");
  data.audio_filesink9 = gst_element_factory_make("filesink","audio_filesink9");
  g_object_set (data.audio_filesink9 , "location", "client1_pcm.wav", NULL);
  data.audio_queue91 = gst_element_factory_make("queue", "audio_queue91");
  data.audio_queue92 = gst_element_factory_make("queue", "audio_queue92");
  data.audio_volume9 = gst_element_factory_make("volume", "audio_volume9");
  data.audio_convert9 = gst_element_factory_make("audioconvert", "audio_convert9");
  data.audio_rtp9 = gst_element_factory_make("rtpL16depay", "audio_rtp9");
  data.audio_sink9 = gst_element_factory_make("alsasink", "audio_sink9");
  //g_object_set (data.audio_sink3 , "device", "hw:0", NULL);
  data.pipeline9 = gst_pipeline_new ("pipeline9");
  if (!data.pipeline9 || !data.audio_source9 || !data.audio_rtp9 || !data.audio_convert9 || !data.audio_sink9 || !data.tee9 || !data.audio_queue91 || !data.audio_queue92 || !data.audio_wavenc9              || !data.audio_filesink9 ) {
    g_printerr ("Not all elements pl9 could be created.\n");
    return ;
  }

  gst_bin_add_many (GST_BIN (data.pipeline9), data.audio_source9, data.audio_rtp9, data.audio_convert9, data.tee9, data.audio_queue91, data.audio_sink9, data.audio_volume9, NULL);
  gst_bin_add_many (GST_BIN (data.pipeline9), data.audio_queue92, data.audio_wavenc9, data.audio_filesink9, NULL);
  
 
  if (!gst_element_link_many (data.audio_source9, data.audio_rtp9,  data.audio_convert9, data.tee9, data.audio_queue91, data.audio_volume9,  data.audio_sink9,NULL) 
      || !gst_element_link_many (data.tee9, data.audio_queue92, data.audio_wavenc9, data.audio_filesink9, NULL) ) {
    g_printerr ("Elements in pl9 could not be linked.\n");
    gst_object_unref (data.pipeline9);
    return ;
  }

  







}


//---------------------------------------END------OF---------PCM--------------------------------------------------------------------------------------------------







//--------------------------------------------OOOOOOOOOOOPPPPPPPPPPPPPPPPPPPPPUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUSSSSSSSSSSSSSSSSSSSSSSSSSSSSS-----------------------------------
static void pipelines_opus()
{




//-------------------------BUILD THE PIPELINE10------START---------YOUR DIRECT SOUND---LISTEN AND RECORD---------------------------------



//                                QUEUE------OPUSENC----OPUSDEC----ALSASINK(LISTEN YOUR SOUND AFTER OPUS ENCODE DECODE)
//-------ALSASRC--------TEE------|
//				  QUEUE------WAVENC---------FILESINK(WRITE TO FILE AS WAV)





   data.audio_source10 = gst_element_factory_make("alsasrc", "audio_source10");
  //g_object_set (data.audio_source4 , "device", "hw:0", NULL);
   data.audio_volume10 = gst_element_factory_make("volume", "audio_volume10");
  data.audio_opusenc10 = gst_element_factory_make("opusenc", "audio_opusenc10");
  data.audio_opusdec10 = gst_element_factory_make("opusdec", "audio_opusdec10");
  data.audio_sink10 = gst_element_factory_make("alsasink", "audio_sink10");
  //g_object_set (data.audio_sink4 , "device", "hw:0", NULL);
  data.pipeline10 = gst_pipeline_new ("pipeline10");
  if (!data.pipeline10 || !data.audio_source10 || !data.audio_volume10 || !data.audio_opusenc10 || !data.audio_opusdec10 || !data.audio_sink10  
      || !data.audio_opusdec10 ) {
    g_printerr ("Not all elements could be created.\n");
    //return ;
  }
  gst_bin_add_many (GST_BIN (data.pipeline10),  data.audio_source10, data.audio_volume10, data.audio_sink10,data.audio_opusenc10,data.audio_opusdec10, NULL);

  if (!gst_element_link_many (data.audio_source10,data.audio_volume10,data.audio_opusenc10, data.audio_opusdec10,data.audio_sink10, NULL)) {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (data.pipeline4);
    //return ;
  }
  



//-------------------------BUILD THE PIPELINE11------START---------YOUR DIRECT SOUND----RECORDED-------------------------------

   data.audio_source8 = gst_element_factory_make("alsasrc", "audio_source8");
  //g_object_set (data.audio_source8 , "device", "hw:0", NULL);
  data.audio_opusenc8 = gst_element_factory_make("opusenc", "audio_opusenc8");
  data.audio_opusdec8 = gst_element_factory_make("opusdec", "audio_opusdec8");
  data.audio_wavenc8 = gst_element_factory_make("wavenc","audio_wavenc8");
  data.audio_filesink8 = gst_element_factory_make("filesink","audio_filesink8");
  g_object_set (data.audio_filesink8 , "location", "out_opus.wav", NULL);
  data.pipeline8 = gst_pipeline_new ("pipeline8");
  if (!data.pipeline8 || !data.audio_source8 || !data.audio_opusenc8 || !data.audio_opusdec8 || !data.audio_filesink8  || !data.audio_wavenc8 ) {
    g_printerr ("Not all elements could be created.\n");
    //return ;
  }
  gst_bin_add_many (GST_BIN (data.pipeline8),  data.audio_source8, data.audio_opusenc8, data.audio_opusdec8,data.audio_wavenc8,data.audio_filesink8, NULL);

  if (!gst_element_link_many (data.audio_source8, data.audio_opusenc8, data.audio_opusdec8,data.audio_wavenc8,data.audio_filesink8, NULL)) {
    g_printerr ("Elements could not be linked.\n");
    gst_object_unref (data.pipeline8);
    //return ;
  }






//-------------------------BUILD THE PIPELINE12------START-----------STREAM TO PEER----------------------------------

//-------ALSASRC---------opusenc-----rtpopuspay-------UDPSINK(SEND TO NETWORK)

  data.audio_source5 = gst_element_factory_make("alsasrc", "audio_source5");
  //g_object_set (data.audio_source5 , "device", "hw:0", NULL);
  data.audio_opusenc5 = gst_element_factory_make("opusenc", "audio_opusenc5");
  data.audio_rtpopuspay5 = gst_element_factory_make("rtpopuspay", "audio_rtpopuspay5");
  data.audio_udp5 = gst_element_factory_make("udpsink", "audio_udp5");

  data.pipeline5 = gst_pipeline_new ("pipeline5");
  if (!data.pipeline5 || !data.audio_source5 || !data.audio_opusenc5 || !data.audio_rtpopuspay5 || !data.audio_udp5  ) {
    g_printerr ("Not all pl5 elements could be created.\n");
    return ;
  }
  gst_bin_add_many (GST_BIN (data.pipeline5), data.audio_source5, data.audio_opusenc5, data.audio_rtpopuspay5, data.audio_udp5, NULL);
  if (!gst_element_link_many (data.audio_source5, data.audio_opusenc5, data.audio_rtpopuspay5, data.audio_udp5,NULL)) {
    g_printerr ("Elements  pl5 could not be linked.\n");
    gst_object_unref (data.pipeline5);
    return ;
  }




//-------------------------BUILD THE PIPELINE13------START----------STREAM FROM PEER-----------LISTEN---AND RECORD------------------------



//                                                           QUEUE------VOLUME-----ALSASINK(LISTEN PEER'S SOUND)
//-------UDPSRC------rtpopusdepay---opusdec----TEE------|
//				                             QUEUE------WAVENC-----FILESINK(WRITE TO FILE)


  data.tee6 = gst_element_factory_make("tee","tee6");
  data.audio_queue61 = gst_element_factory_make("queue", "audio_queue61");
  data.audio_queue62 = gst_element_factory_make("queue", "audio_queue62");
  data.audio_wavenc6 = gst_element_factory_make("wavenc","audio_wavenc6");
  data.audio_filesink6 = gst_element_factory_make("filesink","audio_filesink6");
  g_object_set (data.audio_filesink6 , "location", "in_opus.wav", NULL);


  data.caps1 = gst_caps_from_string("application/x-rtp, media=(string)audio, payload=(int)127, clock-rate=(int)48000, encoding-params=(string)2, encoding-name=(string)X-GST-OPUS-DRAFT-SPITTKA-00");
  data.audio_source6 = gst_element_factory_make("udpsrc", "audio_source6");
  g_object_set (data.audio_source6 , "port",7151 ,"caps", data.caps1 , NULL);
  gst_caps_unref(data.caps);


  data.audio_volume6 = gst_element_factory_make("volume", "audio_volume6");
  data.audio_opusdec6 = gst_element_factory_make("opusdec", "audio_opusdec6");
  data.audio_rtpopusdepay6 = gst_element_factory_make("rtpopusdepay", "audio_rtpopusdepay6");
  data.audio_sink6 = gst_element_factory_make("alsasink", "audio_sink6");
  //g_object_set (data.audio_sink6 , "device", "hw:0", NULL);
  data.pipeline6 = gst_pipeline_new ("pipeline6");
  if (!data.pipeline6 || !data.audio_source6 || !data.audio_rtpopusdepay6 || !data.audio_opusdec6 || !data.audio_sink6 || !data.audio_volume6 
	|| !data.tee6 || !data.audio_queue61 || !data.audio_queue62 || !data.audio_wavenc6    || !data.audio_filesink6  ) {
    g_printerr ("Not all elements pl6 could be created.\n");
    return ;
  }

  gst_bin_add_many (GST_BIN (data.pipeline6), data.audio_source6, data.audio_rtpopusdepay6, data.audio_opusdec6, data.tee6, data.audio_queue61, data.audio_sink6, data.audio_volume6, NULL);
  gst_bin_add_many (GST_BIN (data.pipeline6), data.audio_queue62, data.audio_wavenc6, data.audio_filesink6, NULL);

  
 
  if (!gst_element_link_many (data.audio_source6, data.audio_rtpopusdepay6,  data.audio_opusdec6, data.tee6, data.audio_queue61,data.audio_volume6,  data.audio_sink6,NULL)
	|| !gst_element_link_many (data.tee6, data.audio_queue62, data.audio_wavenc6, data.audio_filesink6, NULL)) 
  {
    g_printerr ("Elements in pl6 could not be linked.\n");
    gst_object_unref (data.pipeline6);
    return ;
  }

  
//-------------------------BUILD THE PIPELINE6------END---------------------------------------------  







}
//--------------------------------------------OOOOOOOOOOOPPPPPPPPPPPPPPPPPPPPPUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUSSSSSSSSSSS-----------END-----------------------























//----------------------------------------------------------------------------------------------------------------

//ip and port submit button callback

static void ip_port( GtkButton *button, GObject *context_object )
{

    GtkLabel *accept_lable1 = g_object_get_data (context_object, "label6");
    GtkLabel *accept_lable2 = g_object_get_data (context_object, "label7");
    GtkEntry *accept_entry1 = g_object_get_data (context_object, "entry1");
    GtkEntry *accept_entry2 = g_object_get_data (context_object, "entry2");
    ip = gtk_entry_get_text (accept_entry1);
    port =atoi( gtk_entry_get_text (accept_entry2));
    g_object_set (data.audio_udp2 , "host", ip, "port",port, NULL);
    g_object_set (data.audio_udp5 , "host", ip, "port",port, NULL);

    g_print("host_ip= %s and port = %d \n", ip, port);
  //ektipwsi sto gui-------------------------------------
    const char *entry1_in = gtk_entry_get_text (accept_entry1);
    gtk_label_set_text (accept_lable1, entry1_in);
    const char *entry2_in = gtk_entry_get_text (accept_entry2);
    gtk_label_set_text (accept_lable2, entry2_in);
  
}


//-------------------------BUFFER CHANGE----------------------------------------------------------------

static void change_buffer (GtkButton *button, GObject *context_object) 
{

	gint64 buf_size;
	const gchar *buf;

	GtkEntry *accept_entry4 = g_object_get_data (context_object, "entry4");        
        buf = gtk_entry_get_text (accept_entry4);
	buf_size = atoi(gtk_entry_get_text (accept_entry4));
        g_print ("Buffer-size: %"G_GUINT64_FORMAT" us\n", buf_size);
        g_object_set (data.audio_source1, "buffer-time", buf_size, NULL);
        g_object_set (data.audio_source2, "buffer-time", buf_size, NULL);
        g_object_set (data.audio_source4, "buffer-time", buf_size, NULL);
        g_object_set (data.audio_source5, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_source7, "buffer-time", buf_size, NULL);
        g_object_set (data.audio_source8, "buffer-time", buf_size, NULL);
        g_object_set (data.audio_sink1, "buffer-time", buf_size, NULL);
        g_object_set (data.audio_sink2, "buffer-time", buf_size, NULL);
        g_object_set (data.audio_sink3, "buffer-time", buf_size, NULL);
        g_object_set (data.audio_sink5, "buffer-time", buf_size, NULL);
        g_object_set (data.audio_sink6, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_sink7, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_sink8, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_sink9, "buffer-time", buf_size, NULL);

        


}

//-----------------------------------------------------------------------------------------------------------


//-------------------------SAMPLE RATE----------------------------------------------------------------

static void sample_rate (GtkButton *button, GstElement *source, GObject *context_object ) //, GObject *context_object
{



	gint64 sam_rate;
	const gchar *sample_r;

	GtkEntry *accept_entry3 = g_object_get_data (context_object, "entry3");        
        sample_r = gtk_entry_get_text (accept_entry3);
	sam_rate = atoi(gtk_entry_get_text (accept_entry3));

        g_print ("Sample-rate: %"G_GUINT64_FORMAT" Hz\n", sam_rate);
        data.audio_pad1 = gst_element_get_static_pad(data.audio_source1, "src");
        if (!data.audio_pad1) {
    		g_printerr ("Could not retrieve pad '%s'\n", "audio_source1");
    		//return 0;
  	}
        gst_pad_use_fixed_caps(data.audio_pad1);	
        g_print ("Sample-rate: %"G_GUINT64_FORMAT" Hz\n", sam_rate);
	data.caps = gst_caps_new_simple("audio/x-raw",
                                        "format", G_TYPE_STRING, "L16",
                                        "rate", G_TYPE_INT, sam_rate,
	                	   "width", G_TYPE_INT, 8,
				   "depth", G_TYPE_INT, 16,
				   "signed", G_TYPE_BOOLEAN, FALSE,
                                   "channels", G_TYPE_INT, 2, NULL );
        g_print ("Sample-rate: %"G_GUINT64_FORMAT" Hz\n", sam_rate);
	//gst_pad_set_caps (data.audio_pad1, data.caps);
        if(!gst_pad_set_caps (data.audio_pad1, data.caps))
	{      
				g_printerr ("Error setting caps: %s\n", error->message);
				return ;
	}
 	gst_object_unref (data.audio_pad1);
}

//-----------------------------------------------------------------------------------------------------------





/* This function is called when the PLAY1 button is clicked */
static void play_cb1 () {
  if  (pcm == 0 && opus == 1 )
	{

	        gst_element_set_state (data.pipeline1, GST_STATE_PLAYING);
  		g_print("Your drirect sound is playing in PCM format pcm = %d and opus = %d\n" ,pcm , opus);
	}
      else 
	{
	        gst_element_set_state (data.pipeline4, GST_STATE_PLAYING);		
  		g_print("Your drirect sound is playing in OPUS format pcm = %d and opus = %d\n" ,pcm , opus);		
        }


}


/* This function is called when the PAUSE button is clicked */
static void pause_cb1 () {
  gst_element_set_state (data.pipeline1, GST_STATE_PAUSED);
}


/* This function is called when the STOP button is clicked */
static void stop_cb1 () {

	        gst_element_set_state (data.pipeline1, GST_STATE_NULL);
	        gst_element_set_state (data.pipeline4, GST_STATE_NULL);		
  if  (pcm == 0 && opus == 1 )
	{

 		g_print("Your drirect sound in PCM format is stopped pcm = %d and opus = %d\n" ,pcm , opus);
	}
      else 
	{
	        gst_element_set_state (data.pipeline4, GST_STATE_NULL);		
  		g_print("Your drirect sound in OPUS format is stopped pcm = %d and opus = %d\n" ,pcm , opus);		
        }


}


/* This function is called when the PLAY2 button is clicked */
static void play_cb2 () {
  if  (pcm == 0 && opus == 1 )
	{

	        gst_element_set_state (data.pipeline2, GST_STATE_PLAYING);
  		g_print("Your  sound to peer is playing in PCM format pcm = %d and opus = %d\n" ,pcm , opus);
	}
      else 
	{
	        gst_element_set_state (data.pipeline5, GST_STATE_PLAYING);		
  		g_print("Your sound to peer is playing in OPUS format pcm = %d and opus = %d\n" ,pcm , opus);		
        }
}


/* This function is called when the PAUSE2 button is clicked */
static void pause_cb2 () {
  gst_element_set_state (data.pipeline2, GST_STATE_PAUSED);
}     	


/* This function is called when the STOP2 button is clicked */
static void stop_cb2 () {
  gst_element_set_state (data.pipeline2, GST_STATE_NULL);
  gst_element_set_state (data.pipeline5, GST_STATE_NULL);
  g_print("Sound to peer is stoped\n");
}
  



/* This function is called when the PLAY3 button is clicked */
static void play_cb3 () {
  gst_element_set_state (data.pipeline3, GST_STATE_PLAYING);
  g_print("Your Sound from server is playing\n");
}
  
/* This function is called when tgcc pipeline.c -o pipeline `pkg-config --cflags --libs gtk+-2.0 gstreamer-interfaces-0.10 gstreamer-1.0`he PAUSE3 button is clicked */
static void pause_cb3 () {
  gst_element_set_state (data.pipeline3, GST_STATE_PAUSED);
}

  
/* This function is called when the STOP3 button is clicked */
static void stop_cb3 () {
  gst_element_set_state (data.pipeline3, GST_STATE_READY);
  g_print("Your Sound from server is stopped\n");
}



/* This function is called when the PLAY4 button is clicked */
static void play_cb4 () {
  gst_element_set_state (data.pipeline4, GST_STATE_PLAYING);
  g_print("Client's 2 sound is playing\n");
}
  
/* This function is called when tgcc pipeline.c -o pipeline `pkg-config --cflags --libs gtk+-2.0 gstreamer-interfaces-0.10 gstreamer-1.0`he PAUSE3 button is clicked */
static void pause_cb4 () {
  gst_element_set_state (data.pipeline4, GST_STATE_PAUSED);
}

  
/* This function is called when the STOP3 button is clicked */
static void stop_cb4 () {
  gst_element_set_state (data.pipeline4, GST_STATE_READY);
  g_print("Client's 2 sound is stoped\n");
}








static void
print_hello (GtkWidget *button,
             GObject *context_object)
{
  g_print ("Hello World\n");
}


//--------------------------------VOLUME FUNCTIONSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS------------

static void change_volume1 (GtkScaleButton * volume_1, GstElement * volume){
  	gdouble value1;


        value1 = gtk_scale_button_get_value (volume_1);
        g_print ("Volume1: %f dB\n", value1);
        g_object_set (data.audio_volume1, "volume", value1, NULL);
}

static void change_volume2 (GtkScaleButton * volume_2, GstElement * volume){
  	gdouble value2;


        value2 = gtk_scale_button_get_value (volume_2);
        g_print ("Volume2: %f dB\n", value2);
        g_object_set (data.audio_volume3, "volume", value2, NULL);
}


static void change_volume3 (GtkScaleButton * volume_3, GstElement * volume){
  	gdouble value3;


        value3 = gtk_scale_button_get_value (volume_3);
        g_print ("Volume3: %f dB\n", value3);
        g_object_set (data.audio_volume4, "volume", value3, NULL);
}


static void change_volume4 (GtkScaleButton * volume_4, GstElement * volume){
  	gdouble value4;


        value4 = gtk_scale_button_get_value (volume_4);
        g_print ("Volume4: %f dB\n", value4);
        g_object_set (data.audio_volume5, "volume", value4, NULL);
}




void buttons(){
    


  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "builder-srv-clnt.ui", &error) == 0)
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

  g_signal_connect (GTK_BUTTON(submit), "clicked", G_CALLBACK (ip_port), submit);  



  

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




  button = gtk_builder_get_object (builder, "play4");
  g_signal_connect (button, "clicked", G_CALLBACK (play_cb4), NULL);

  button = gtk_builder_get_object (builder, "pause4");
  g_signal_connect (button, "clicked", G_CALLBACK (pause_cb4), NULL);

  button = gtk_builder_get_object (builder, "stop4");
  g_signal_connect (button, "clicked", G_CALLBACK (stop_cb4), NULL);



  
//-----------------------VOLUME---BUTTONSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS



  volume1 = gtk_builder_get_object (builder, "volumebutton1");
  g_signal_connect (volume1, "value-changed",G_CALLBACK (change_volume1), NULL);



  volume2 = gtk_builder_get_object (builder, "volumebutton2");
  g_signal_connect (volume2, "value-changed",G_CALLBACK (change_volume2), NULL);


  volume3 = gtk_builder_get_object (builder, "volumebutton3");
  g_signal_connect (volume3, "value-changed",G_CALLBACK (change_volume3), NULL);



  volume4 = gtk_builder_get_object (builder, "volumebutton4");
  g_signal_connect (volume4, "value-changed",G_CALLBACK (change_volume4), NULL);
//-----------------------VOLUME---BUTTONSSSSSSSS-------END-------------------------

//----------------------BUFFER SIZE INPUT----------------------------
  entry4 = gtk_builder_get_object (builder, "entry4");
  change_buf = gtk_builder_get_object (builder, "button2");
    g_object_set_data(G_OBJECT(change_buf), "entry4", entry4);
  g_signal_connect (GTK_BUTTON(change_buf) , "clicked", G_CALLBACK (change_buffer), change_buf);
//---------------------------------------------------------------------------------------------


//----------------------sample rate----------------------------

  entry3 = gtk_builder_get_object (builder, "entry3");
  samp_rate = gtk_builder_get_object (builder, "button1");
    g_object_set_data(G_OBJECT(samp_rate), "entry3", entry3);
  g_signal_connect (GTK_BUTTON(samp_rate) , "clicked", G_CALLBACK (sample_rate), samp_rate);
//---------------------------------------------------------------------------------------------




}





int main (int   argc,   char *argv[])
{
  pcm = 0;
  opus = 1;

  /* Initialize GTK */
  gtk_init (&argc, &argv);
  /* Initialize GStreamer */
  gst_init (&argc, &argv);
  
  pipelines_pcm();
  pipelines_opus();








  buttons();
  
  gtk_main ();

  return 0;
}


#include <gtk/gtk.h>
//p2p audio application based on gstreamer and gtk
/*gcc p2p-stream-caps-sr.c -o Aretousa_v8 `pkg-config --cflags --libs gtk+-3.0 gstreamer-interfaces-0.10 gstreamer-0.10`
gcc p2p-stream-caps.c -o p2p-stream-caps `pkg-config --cflags --libs gtk+-3.0 gstreamer-1.0`

23-4-2020
gcc aretousa-pulse-modules.c -o aretousa-pulse-modules `pkg-config --cflags --libs gtk+-3.0 gstreamer-1.0`

*/
#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>
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
  GstElement *pipeline1, *pipeline2, *pipeline3, *pipeline4,*pipeline5,*pipeline6,*pipeline7,*pipeline8,*pipeline9, *pipeline10, *pipeline11,  *pipeline12;           /* pipelines initialisation */
  GstElement *pl_cam,*pl_send_cam, *pl_receive_cam ; 
  GstElement 	    *audio_source1 ,*capsfilter1, *audio_sink1 ,*tee1 , *audio_queue11,*audio_queue12, *video_queue , *video_convert, *video_sink,
	     *audio_udp1 , *audio_convert1, *audio_rtp1, *audio_volume1, *audio_opusenc1, *audio_opusdec1, *factory,*audio_rtpopusenc1, *audio_wavenc1, *audio_filesink1, *audioresample1,

		*audio_source2 ,*capsfilter2, *audio_sink2 , *visual2 ,*tee2 , *audio_queue21,*audio_queue22, *video_queue2 , *video_convert2, *video_sink2,
	     *audio_udp2 , *audio_convert2, *audio_rtp2, *audio_volume2, *audio_opusenc2, *audio_rtpopuspay2, *audio_wavenc2, *audio_filesink2, *audioresample2,

		*audio_source3 ,*capsfilter3, *audio_sink3 , *visual3 ,*tee3 , *audio_queue31,*audio_queue32, *video_queue3 , *video_convert3, *video_sink3,
	     *audio_udp3 , *audio_convert3, *audio_rtp3 , *audio_volume3, *audio_opusdec3, *audio_rtpopusenc3, *audio_rtpopusdepay3,*audio_wavenc3, *audio_filesink3, *audioresample3,

		*audio_source4 , *capsfilter4, *audio_sink4 , *visual4 ,*tee4 , *audio_queue41,*audio_queue42, *video_queue4 , *video_convert4, *video_sink4,*audio_wavenc4, *audio_filesink4,
	     *audio_udp4 , *audio_convert4, *audio_rtp4 , *audio_volume4, *audio_opusenc4, *audio_opusdec4, *audio_rtpopusenc4, *audio_rtpopusdepay4,

		*audio_source5 ,*capsfilter5, *audio_sink5 , *visual5 ,*tee5 , *audio_queue5, *video_queue5 , *video_convert5, *video_sink5,
	     *audio_udp5 , *audio_convert5, *audio_rtp5 , *audio_volume5, *audio_opusenc5,*audio_opusdec5, *audio_rtpopusenc5, *audio_rtpopuspay5,*audio_rtpopusdepay5,

		*audio_source6 ,*capsfilter6, *audio_sink6 , *visual6 ,*tee6 , *audio_queue61,*audio_queue62, *video_queue6 , *video_convert6, *video_sink6,
	     *audio_udp6 , *audio_convert6, *audio_rtp6 , *audio_volume6, *audio_opusdec6, *audio_rtpopusenc6, *audio_rtpopusdepay6,*audio_wavenc6, *audio_filesink6, *audioresample6,

		*audio_source7 ,*capsfilter7, *audio_sink7 , *visual7 ,*tee7 , *audio_queue7, *video_queue7 , *video_convert7, *video_sink7, *audio_interleave7,
	     *audio_udp7 , *audio_convert7, *audio_rtp7 , *audio_volume7, *audio_opusdec7, *audio_rtpopusenc7, *audio_rtpopusdepay7,*audio_wavenc7, *audio_filesink7, *audioresample7,

		*audio_source8 , *capsfilter8,*audio_sink8 , *visual8 ,*tee8 , *audio_queue8, *video_queue8 , *video_convert8, *video_sink8,*audio_wavenc8, *audio_filesink8,
	     *audio_udp8 , *audio_convert8, *audio_rtp8 , *audio_volume8, *audio_opusdec8,*audio_opusenc8, *audio_rtpopusenc8, *audio_rtpopusdepay8,

		*audio_source9 , *capsfilter9, *audio_sink9 , *visual9 ,*tee9 , *audio_queue91, *audio_queue92 , *video_convert9, *video_sink9,
	     *audio_udp9 , *audio_convert9, *audio_rtp9 , *audio_volume9, *audio_opusdec9, *audio_rtpopusenc9, *audio_rtpopusdepay9,*audio_wavenc9, *audio_filesink9, *audioresample9,


		*audio_source10 ,*capsfilter10, *audio_sink10 , *visual10 ,*tee10 , *audio_queue101, *audio_queue102 , *video_convert10, *video_sink10,
	     *audio_udp10 , *audio_convert10, *audio_rtp10 , *audio_volume10, *audio_opusdec10, *audio_rtpopusenc10, *audio_rtpopusdepay10,*audio_wavenc10, *audio_filesink10, *audioresample10,
		
		*audio_source11 ,*capsfilter11, *audio_sink11 , *visual11 ,*tee11 , *audio_queue111, *audio_queue112 , *video_convert11, *video_sink11,
	     *audio_udp11 , *audio_convert11, *audio_rtp11 , *audio_volume11, *audio_opusdec11, *audio_rtpopusenc11, *audio_rtpopusdepay11,*audio_wavenc11, *audio_filesink11, *audioresample11;

             
  GstPad *audio_pad1;
  GstStructure *structure;
  GstCaps *alsa_caps;
  GstCaps *caps, *caps1 ,*caps2, *caps3, *caps4 ;      		  /*Caps*/
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

  GObject *toggle1;
  GObject *toggle2;
  GObject *volume1; 
  GObject *volume2;
  GObject *volume3;
  GObject *samp_rate;
  GError *error = NULL;
  GtkBuilder *builder;
  GObject *window;
  GObject *button, *change_buf, *pac_sz, *opus_set_br, *opus_set_fs, *opus_set_compl,*opus_bw, *opus_cbr;
  GObject *submit;
  GObject *label5;
  GObject *label6;
  GObject *label7;
  GObject *entry1;
  GObject *entry2;
  GObject *entry3;
  GObject *entry4;
  GObject *entry5;
  GObject *entry6;
  GObject *entry7;
  GObject *entry8;
  GObject *entry9;
  GObject *entry10;
  GObject *entry11;
  GObject *entry_text1;
  GObject *entry_text2;
  GtkWidget *main_hbox; 
  GtkEntry *ip_entry;
  GtkEntry *port_entry;
  const gchar *ip;
  gint port1, port2;
  const gchar * s_r ;
  gint opus,pcm, format;
  gint64 buf_size ;
  GObject *context_object;
  GObject *context_object_buf;
  CustomData data;
  GtkToggleButton *togglebutton;
  gint64 pack_s;
  const gchar *p_s;


  //char command[100];
  //strcpy(command, "gnome-control-center sound");
  //system(command);
//----------------------------------------------------------------------------------------------------------------


void choose_format_pcm(GtkToggleButton *source, gpointer user_data)
{
	  pcm = 0; opus = 1;
	  format = gtk_toggle_button_get_active (source);
	  g_printf ("pcm is %d and opus is %d\n", pcm , opus);
	  
}

static void choose_format_opus(GtkToggleButton *source, gpointer user_data)
{	
	  pcm = 1; opus = 0;
	  format = gtk_toggle_button_get_active (source);
	 
	  g_printf ("pcm is %d and opus is %d\n", pcm , opus);




}

//-------------------------SAMPLE RATE----------------------------------------------------------------

static void sample_rate (GtkButton *button, GstElement *source, GObject *context_object ) //, GObject *context_object
{



	gint64 sam_rate;
	const gchar *sample_r;

	GtkEntry *accept_entry3 = g_object_get_data (context_object, "entry3");        
        sample_r = gtk_entry_get_text (accept_entry3);
	sam_rate = atoi(gtk_entry_get_text (accept_entry3));
	s_r = sample_r;

        g_print ("Sample-rate: %"G_GUINT64_FORMAT" Hz\n", sam_rate);




}

//-----------------------------------------------------------------------------------------------------------



//ip and port submit button callback

static void ip_port( GtkButton *button, GObject *context_object )
{

    GtkLabel *accept_lable1 = g_object_get_data (context_object, "label6");
    GtkLabel *accept_lable2 = g_object_get_data (context_object, "label7");
    GtkLabel *accept_lable5 = g_object_get_data (context_object, "label5");
    GtkEntry *accept_entry1 = g_object_get_data (context_object, "entry1");
    GtkEntry *accept_entry2 = g_object_get_data (context_object, "entry2");
    GtkEntry *accept_entry10 = g_object_get_data (context_object, "entry10");
    ip = gtk_entry_get_text (accept_entry1);
    port1 =atoi( gtk_entry_get_text (accept_entry2));
    port2 =atoi( gtk_entry_get_text (accept_entry10));
    g_object_set (data.audio_udp2 , "host", ip, "port",port1, NULL);
    g_object_set (data.audio_udp5 , "host", ip, "port",port1, NULL);
    g_object_set (data.audio_udp9 , "host", ip, "port",port2, NULL);
    g_object_set (data.audio_udp6 , "host", ip, "port",port2, NULL);
    g_print("host_ip= %s and port1 = %d and port2=%d \n", ip, port1, port2);
  //ektipwsi sto gui-------------------------------------
    const char *entry1_in = gtk_entry_get_text (accept_entry1);
    gtk_label_set_text (accept_lable1, entry1_in);
    const char *entry2_in = gtk_entry_get_text (accept_entry2);
    gtk_label_set_text (accept_lable2, entry2_in);
    const char *entry5_in = gtk_entry_get_text (accept_entry10);
    gtk_label_set_text (accept_lable5, entry5_in);
  
}




//-------OPUS---SET-----BIT------RATE-------------------------------------------

static void set_bit_rate(GtkButton *button, GObject *context_object)
{
	gint64 bit_rate;
	const gchar *br;
	GtkEntry *accept_entry7 = g_object_get_data (context_object, "entry7");        
        br = gtk_entry_get_text (accept_entry7);
	bit_rate = atoi(gtk_entry_get_text (accept_entry7));
        g_object_set (data.audio_opusenc4, "bitrate", bit_rate, NULL);
        //g_object_set (data.audio_opusdec4, "bitrate", bit_rate, NULL);
        g_object_set (data.audio_opusenc8, "bitrate", bit_rate, NULL);
        //g_object_set (data.audio_opusdec8, "bitrate", bit_rate, NULL);
        g_object_set (data.audio_opusenc5, "bitrate", bit_rate, NULL);
        //g_object_set (data.audio_opusdec6, "bitrate", bit_rate, NULL);

        g_print ("Opus bit-rate: %"G_GUINT64_FORMAT" \n", bit_rate);

}


//-------OPUS---SET-----FRAME-SIZE-------------------------------------------

static void set_frame_size(GtkButton *button, GObject *context_object)
{
	gint64 frame_size;
	const gchar *fs;
	GtkEntry *accept_entry6 = g_object_get_data (context_object, "entry6");        
        fs = gtk_entry_get_text (accept_entry6);
	frame_size = atoi(gtk_entry_get_text (accept_entry6));
        g_object_set (data.audio_opusenc4, "frame-size", frame_size, NULL);

        g_object_set (data.audio_opusenc8, "frame-size", frame_size, NULL);

        g_object_set (data.audio_opusenc5, "frame-size", frame_size, NULL);


        g_print ("Opus frame_size: %"G_GUINT64_FORMAT" \n", frame_size);

}


//-------OPUS---SET-----CBR--VBR----C-VBR-------------------------------------------

static void opus_cbr_vbr(GtkButton *button, GObject *context_object)
{
	gint64 cbr;
	const gchar *cbr_vbr;
	GtkEntry *accept_entry11 = g_object_get_data (context_object, "entry11");        
        cbr_vbr = gtk_entry_get_text (accept_entry11);
	cbr = atoi(gtk_entry_get_text (accept_entry11));
        g_object_set (data.audio_opusenc4, "bitrate-type", cbr, NULL);

        g_object_set (data.audio_opusenc8, "bitrate-type", cbr, NULL);

        g_object_set (data.audio_opusenc5, "bitrate-type", cbr, NULL);


        g_print ("Opus cbr vbr cvbr: %"G_GUINT64_FORMAT" \n", cbr);

}



//-------OPUS---SET-----COMPLEXITY-------------------------------------------

static void set_complexity(GtkButton *button, GObject *context_object)
{
	gint64 compl;
	const gchar *complexity;
	GtkEntry *accept_entry9 = g_object_get_data (context_object, "entry9");        
        complexity = gtk_entry_get_text (accept_entry9);
	compl = atoi(gtk_entry_get_text (accept_entry9));
        //g_object_set (data.audio_opusenc4, "complexity", compl, NULL);

        //g_object_set (data.audio_opusenc8, "frame-size", compl, NULL);

        //g_object_set (data.audio_opusenc5, "frame-size", compl, NULL);


        //g_print ("Opus complexity: %"G_GUINT64_FORMAT" \n", compl);

}

static void opus_bandwidth(GtkButton *button, GObject *context_object)
{
	gint64 bandwidth;
	const gchar *bw;
	GtkEntry *accept_entry8 = g_object_get_data (context_object, "entry8");        
        bw = gtk_entry_get_text (accept_entry8);
	bandwidth = atoi(gtk_entry_get_text (accept_entry8));
        //g_object_set (data.audio_opusenc4, "bandwidth", bandwidth, NULL);

        //g_object_set (data.audio_opusenc8, "bandwidth", bandwidth, NULL);

        //g_object_set (data.audio_opusenc5, "bandwidth", bandwidth, NULL);


        //g_print ("Opus bandwidth: %"G_GUINT64_FORMAT" \n", bandwidth);

}




//-------------------------BUFFER CHANGE----------------------------------------------------------------

static void change_buffer (GtkButton *button, GObject *context_object) 
{

	//gint64 buf_size;
	const gchar *buf;

	GtkEntry *accept_entry4 = g_object_get_data (context_object, "entry4");        
        buf = gtk_entry_get_text (accept_entry4);
	buf_size = atoi(gtk_entry_get_text (accept_entry4));
        g_print ("Buffer-size: %"G_GUINT64_FORMAT" us\n", buf_size);
        //g_object_set (data.audio_source1, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_source2, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_source4, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_source5, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_source7, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_source8, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_sink1, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_sink2, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_sink3, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_sink4, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_sink6, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_sink9, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_sink10, "buffer-time", buf_size, NULL);
        //g_object_set (data.audio_sink11, "buffer-time", buf_size, NULL);        


}

//-------------------------------START----RECORDING----------------------------------------------------------------------------


static void start_rec(){

  if  (pcm == 0 && opus == 1 )
	{

	//gst_element_set_state (data.pipeline2, GST_STATE_PLAYING); //first start to send stream to peer
	for(int c = 1; c <= 32767; c++)for (int d = 1; d <= 32767; d++){}//some time delay before recording starts
	//gst_element_set_state (data.pipeline7, GST_STATE_PLAYING); //secondly start recording direct sound
	//gst_element_set_state (data.pipeline3, GST_STATE_PLAYING); //then start recording stream from peer
		g_print("Recording using pcm started\n");
	}
      else 
	{
	//gst_element_set_state (data.pipeline5, GST_STATE_PLAYING);
	for(int k = 1; k <= 32767; k++)for (int l = 1; l <= 32767; l++){}//some time delay before recording starts
	//gst_element_set_state (data.pipeline8, GST_STATE_PLAYING);
	//gst_element_set_state (data.pipeline6, GST_STATE_PLAYING);

		g_print("Recording using opus started\n");
        }

	//g_print("Recording using pcm started\n");


        
}



static void stop_rec(){

	g_print("Recording Stoped\n");
	//gst_element_set_state (data.pipeline7, GST_STATE_NULL);
	//gst_element_set_state (data.pipeline3, GST_STATE_NULL);
	//gst_element_set_state (data.pipeline2, GST_STATE_NULL);
	//gst_element_set_state (data.pipeline6, GST_STATE_NULL);
	//gst_element_set_state (data.pipeline8, GST_STATE_NULL);
	//gst_element_set_state (data.pipeline5, GST_STATE_NULL);
	
}




//-------------------------PACKET SIZE----------------------------------------------------------------

static void packet_size (GtkButton *button, GObject *context_object) 
{

	gint64 pack_s;
	//const gchar *p_s;

	GtkEntry *accept_entry5 = g_object_get_data (context_object, "entry5");        
        p_s = gtk_entry_get_text (accept_entry5);
	pack_s = atoi(gtk_entry_get_text (accept_entry5));
        g_print ("Packet-size: %"G_GUINT64_FORMAT" byte\n", pack_s);
        //g_object_set (data.audio_rtp1, "mtu", pack_s, NULL);
        //g_object_set (data.audio_rtp2, "mtu", pack_s, NULL);

        g_object_set (data.audio_rtpopuspay5, "mtu", pack_s, NULL);


        


}
//-----------------------------------------------------------------------------------------------------------


//Camera buttons----------------------------------------------------------------------------------------------

















/* This function is called when the PLAY1 button is clicked */
static void play_cb1 (GtkToggleButton *tog, gpointer user_data) {
				
	if (gtk_toggle_button_get_active (tog))
	{
  		if  (pcm == 0 && opus == 1 )
		{

			char s_rate[10];
			char command[100];
   			strcpy( s_rate, s_r);
   			strcpy( command, "pactl load-module module-loopback latency_msec=1 " );
                	strcat(command ,s_rate);
			g_print("%s\n",command);
			system(command);
	        	//gst_element_set_state (data.pipeline1, GST_STATE_PLAYING);
  			g_print("Your drirect sound is playing in PCM format pcm = %d and opus = %d\n" ,pcm , opus);
  			g_print("pact loopback-module is loaded");
		}
	        else 
		{


	        	//gst_element_set_state (data.pipeline4, GST_STATE_PLAYING);		
  			g_print("Your drirect sound is playing in OPUS format pcm = %d and opus = %d\n" ,pcm , opus);		
        	}

	     }

  	else 
	{
  
       		char command[100];
  		strcpy( command, "pactl unload-module module-loopback" );
  		system(command);
	        //gst_element_set_state (data.pipeline1, GST_STATE_NULL);
	        //gst_element_set_state (data.pipeline4, GST_STATE_NULL);		
  		if  (pcm == 0 && opus == 1 )
		{

 			g_print("pactl unload-module module-loopback pcm = %d and opus = %d\n" ,pcm , opus);
		}
      		else 
		{
	        	//gst_element_set_state (data.pipeline4, GST_STATE_NULL);		
  			g_print("Your drirect sound in OPUS format is stopped pcm = %d and opus = %d\n" ,pcm , opus);		
        	}

  	}






}
  
/* This function is called when the PAUSE button is clicked NOT*/
static void pause_cb1 () {
  //gst_element_set_state (data.pipeline1, GST_STATE_PAUSED);
}

/* This function is called when the STOP button is clicked */
static void stop_cb1 () {
  
  char command[100];
  strcpy( command, "pactl unload-module module-loopback" );
  system(command);
	        //gst_element_set_state (data.pipeline1, GST_STATE_NULL);
	        //gst_element_set_state (data.pipeline4, GST_STATE_NULL);		
  if  (pcm == 0 && opus == 1 )
	{

 		g_print("pactl unload-module module-loopback pcm = %d and opus = %d\n" ,pcm , opus);
	}
      else 
	{
	        //gst_element_set_state (data.pipeline4, GST_STATE_NULL);		
  		g_print("Your drirect sound in OPUS format is stopped pcm = %d and opus = %d\n" ,pcm , opus);		
        }


}

  

/* This function is called when the PLAY2 button is clicked */
static void play_cb2 (GtkToggleButton *tog, gpointer user_data) {
	if (gtk_toggle_button_get_active (tog))
	{
  		if  (pcm == 0 && opus == 1 )
		{

			char s_rate[15];
			char s_mtu[15];
			char command[200];
   			strcpy( s_rate, s_r);
			strcpy(s_mtu, p_s);
   			strcpy( command, "pactl load-module module-rtp-send source_ip=195.251.253.199 destination_ip=195.251.253.201 " );
                	strcat(command ,s_mtu);
			g_print("%s\n",command);
			strcat(command ,s_rate);
			g_print("%s\n",command);
			system(command);
	        	//gst_element_set_state (data.pipeline1, GST_STATE_PLAYING);
  			//g_print("pact module-rtp-send is loaded PCM format pcm = %d and opus = %d\n" ,pcm , opus);

		}
	        else 
		{


	        	//gst_element_set_state (data.pipeline4, GST_STATE_PLAYING);		
  			g_print("Your drirect sound is playing in OPUS format pcm = %d and opus = %d\n" ,pcm , opus);		
        	}

	     }

  	else 
	{
  
       		char command[100];
  		strcpy( command, "pactl unload-module module-rtp-send" );
  		system(command);
	        //gst_element_set_state (data.pipeline1, GST_STATE_NULL);
	        //gst_element_set_state (data.pipeline4, GST_STATE_NULL);		
  		if  (pcm == 0 && opus == 1 )
		{

 			g_print("pactl unload-module module-rtp-send pcm = %d and opus = %d\n" ,pcm , opus);
		}
      		else 
		{
	        	//gst_element_set_state (data.pipeline4, GST_STATE_NULL);		
  			g_print("Your drirect sound in OPUS format is stopped pcm = %d and opus = %d\n" ,pcm , opus);		
        	}

  	}


}
  







/* This function is called when the PAUSE2 button is clicked */
static void pause_cb2 () {
  //gst_element_set_state (data.pipeline2, GST_STATE_PAUSED);
}     	


  
/* This function is called when the STOP2 button is clicked */
static void stop_cb2 () {
		char command[100];
  		strcpy( command, "pactl unload-module module-rtp-send" );
  		system(command);
 		g_print("pactl unload-module module-rtp-send\n");
}
  


/* This function is called when the PLAY3 button is clicked */
static void play_cb3 (GtkToggleButton *tog, gpointer user_data) {
	if (gtk_toggle_button_get_active (tog))
	{
  		if  (pcm == 0 && opus == 1 )
		{

			char s_rate[10];
			char command[100];
   			strcpy( s_rate, s_r);
   			strcpy( command, "pactl load-module module-rtp-recv sap_address=0.0.0.0 latency_msec=1" );//latency_msec=1
                	//strcat(command ,s_rate);
			g_print("%s\n",command);
			system(command);
	        	//gst_element_set_state (data.pipeline1, GST_STATE_PLAYING);
  			g_print("pactl load-module module-rtp-recv sap_address=0.0.0.0 latency_msec=1 PCM format pcm = %d and opus = %d\n" ,pcm , opus);

		}
	        else 
		{


	        	//gst_element_set_state (data.pipeline4, GST_STATE_PLAYING);		
  			g_print("Your drirect sound is playing in OPUS format pcm = %d and opus = %d\n" ,pcm , opus);		
        	}

	     }

  	else 
	{
  
       		char command[100];
  		strcpy( command, "pactl unload-module module-rtp-recv" );
  		system(command);
	        //gst_element_set_state (data.pipeline1, GST_STATE_NULL);
	        //gst_element_set_state (data.pipeline4, GST_STATE_NULL);		
  		if  (pcm == 0 && opus == 1 )
		{

 			g_print("pactl unload-module module-rtp-recv pcm = %d and opus = %d\n" ,pcm , opus);
		}
      		else 
		{
	        	//gst_element_set_state (data.pipeline4, GST_STATE_NULL);		
  			g_print("Your drirect sound in OPUS format is stopped pcm = %d and opus = %d\n" ,pcm , opus);		
        	}

  	}

}
  
/* This function is called when the PAUSE3 button is clicked */
static void pause_cb3 () {
  //gst_element_set_state (data.pipeline3, GST_STATE_PAUSED);
}

  
/* This function is called when the STOP3 button is clicked */
static void stop_cb3 () {
		char command[100];
  		strcpy( command, "pactl unload-module module-rtp-recv" );
  		system(command);
 		g_print("pactl unload-module module-rtp-recv\n");
}




/* This function is called when the 	PLAY4 button is clicked */
static void play_cb4 (GtkToggleButton *tog, gpointer user_data) {
	if (gtk_toggle_button_get_active (tog))
	{




		  if  (pcm == 0 && opus == 1 )
	          {

	        	//gst_element_set_state (data.pipeline10, GST_STATE_PLAYING);
  			g_print("Your  sound from peer is playing in PCM format pcm = %d and opus = %d\n" ,pcm , opus);
		  }
      		  else 
		  {
	        	//gst_element_set_state (data.pipeline11, GST_STATE_PLAYING);		
  			g_print("Your sound from peer is playing in OPUS format pcm = %d and opus = %d\n" ,pcm , opus);		
        	  }
	}
	else
	{
		  //gst_element_set_state (data.pipeline10, GST_STATE_NULL);
		  //gst_element_set_state (data.pipeline11, GST_STATE_NULL);
		  g_print("Sound from peer is stoped\n");
	}
}
  
/* This function is called when the PAUSE4 button is clicked */
static void pause_cb4 () {
  //gst_element_set_state (data.pipeline10, GST_STATE_PAUSED);
}

  
/* This function is called when the STOP4 button is clicked */
static void stop_cb4 () {
  //gst_element_set_state (data.pipeline10, GST_STATE_NULL);
  //gst_element_set_state (data.pipeline11, GST_STATE_NULL);
  g_print("Sound from peer is stoped\n");
}



static void pa_start (GtkToggleButton *tog, gpointer user_data) {
	char command[50];
	if (gtk_toggle_button_get_active (tog)){
		
   		strcpy( command, "pulseaudio --start" );
   		system(command);
        	g_print ("pulseaudio --start\n");
  		//gst_element_set_state (data.pl_cam, GST_STATE_PLAYING);
	     }
  	else {
	  	//gst_element_set_state (data.pl_cam, GST_STATE_NULL);
  	}


}

static void stop_cam () {
  //gst_element_set_state (data.pl_cam, GST_STATE_NULL);
}


static void pa_stop (GtkToggleButton *tog, gpointer user_data) {
	char command[50];
	if (gtk_toggle_button_get_active (tog)){
        	strcpy( command, "pulseaudio -k" );
   		system(command);
        	g_print ("pulseaudio -k\n");
	     }
  	else {
	  	//gst_element_set_state (data.pl_send_cam, GST_STATE_NULL);
  	}


}

static void open_peers_cam (GtkToggleButton *tog, gpointer user_data) {
	char command[50];
	if (gtk_toggle_button_get_active (tog)){
		 strcpy( command, "pavucontrol" );
   		system(command);
        	g_print ("Open peers camera");
  		//gst_element_set_state (data.pl_receive_cam, GST_STATE_PLAYING);
	     }
  	else {
        	g_print ("Stops peers camera");
	  	//gst_element_set_state (data.pl_receive_cam, GST_STATE_PAUSED);
  	}


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
        //g_object_set (data.audio_volume1, "volume", value1, NULL);
        //g_object_set (data.audio_volume4, "volume", value1, NULL);
}

static void change_volume2 (GtkScaleButton * volume_2, GstElement * volume){
  	gdouble value2;


        value2 = gtk_scale_button_get_value (volume_2);
        g_print ("Volume2: %f dB\n", value2);
        //g_object_set (data.audio_volume9, "volume", value2, NULL);
        //g_object_set (data.audio_volume6, "volume", value2, NULL);
}


static void change_volume3 (GtkScaleButton * volume_3, GstElement * volume){
  	gdouble value3;


        value3 = gtk_scale_button_get_value (volume_3);
        g_print ("Volume3: %f dB\n", value3);
        //g_object_set (data.audio_volume10, "volume", value3, NULL);
        //g_object_set (data.audio_volume11, "volume", value3, NULL);
}







void buttons(){
    


  /* Construct a GtkBuilder instance and load our UI description */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, "builder-glade-pulse.ui", &error) == 0)
    {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      //	return 1;
    }

  /* Connect signal handlers to the constructed widgets. */
  window = gtk_builder_get_object (builder, "window");
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
  label5 = gtk_builder_get_object (builder, "label5");
  label6 = gtk_builder_get_object (builder, "label6");
  label7 = gtk_builder_get_object (builder, "label7");
  entry1 = gtk_builder_get_object (builder, "entry1");
  entry2 = gtk_builder_get_object (builder, "entry2");
  entry10 = gtk_builder_get_object (builder, "entry10");
   
  submit = gtk_builder_get_object (builder, "submit");  
    g_object_set_data(G_OBJECT(submit), "label5", label5);
    g_object_set_data(G_OBJECT(submit), "label6", label6);
    g_object_set_data(G_OBJECT(submit), "label7", label7);
    g_object_set_data(G_OBJECT(submit), "entry1", entry1);
    g_object_set_data(G_OBJECT(submit), "entry2", entry2);
    g_object_set_data(G_OBJECT(submit), "entry10", entry10);
  g_signal_connect (GTK_BUTTON(submit), "clicked", G_CALLBACK (ip_port), submit);  



  

  button = gtk_builder_get_object (builder, "play1");
  g_signal_connect (GTK_TOGGLE_BUTTON(button), "toggled", G_CALLBACK (play_cb1), NULL);

  button = gtk_builder_get_object (builder, "pause1");
  g_signal_connect (button, "clicked", G_CALLBACK (pause_cb1), NULL);

  button = gtk_builder_get_object (builder, "stop1");
  g_signal_connect (button, "clicked", G_CALLBACK (stop_cb1), NULL);


  
  button = gtk_builder_get_object (builder, "play2");
  g_signal_connect (GTK_TOGGLE_BUTTON(button), "toggled", G_CALLBACK (play_cb2), NULL);

  button = gtk_builder_get_object (builder, "pause2");
  g_signal_connect (button, "clicked", G_CALLBACK (pause_cb2), NULL);

  button = gtk_builder_get_object (builder, "stop2");
  g_signal_connect (button, "clicked", G_CALLBACK (stop_cb2), NULL);



  button = gtk_builder_get_object (builder, "play3");
  g_signal_connect (GTK_TOGGLE_BUTTON(button), "toggled", G_CALLBACK (play_cb3), NULL);

  button = gtk_builder_get_object (builder, "pause3");
  g_signal_connect (button, "clicked", G_CALLBACK (pause_cb3), NULL);

  button = gtk_builder_get_object (builder, "stop3");
  g_signal_connect (button, "clicked", G_CALLBACK (stop_cb3), NULL);

 

  button = gtk_builder_get_object (builder, "play4");
  g_signal_connect (GTK_TOGGLE_BUTTON(button), "toggled", G_CALLBACK (play_cb4), NULL);

  button = gtk_builder_get_object (builder, "pause4");
  g_signal_connect (button, "clicked", G_CALLBACK (pause_cb4), NULL);

  button = gtk_builder_get_object (builder, "stop4");
  g_signal_connect (button, "clicked", G_CALLBACK (stop_cb4), NULL);


//-----------------------camera-----------buttons------------------------------------
//-----------------------camera-----------buttons------------------------------------


//-----------------test with toggle 5-11-2019-------------------
  button = gtk_builder_get_object (builder, "togglebutton1");
  g_signal_connect (GTK_TOGGLE_BUTTON(button), "toggled", G_CALLBACK (pa_start), NULL);

//-============================================================================





  button = gtk_builder_get_object (builder, "button9");
  g_signal_connect (GTK_TOGGLE_BUTTON(button), "toggled", G_CALLBACK (pa_start), NULL);


  button = gtk_builder_get_object (builder, "button10");
  g_signal_connect (button, "clicked", G_CALLBACK (stop_cam), NULL);

  button = gtk_builder_get_object (builder, "button11");
  g_signal_connect (GTK_TOGGLE_BUTTON(button), "toggled", G_CALLBACK (pa_stop), NULL);
  
  button = gtk_builder_get_object (builder, "button12");
  g_signal_connect (GTK_TOGGLE_BUTTON(button), "toggled", G_CALLBACK (open_peers_cam), NULL);
 


  toggle1 = gtk_builder_get_object (builder, "radiobutton1");
  g_signal_connect (toggle1, "toggled", G_CALLBACK (choose_format_pcm), NULL);

  toggle2 = gtk_builder_get_object (builder, "radiobutton2");
  g_signal_connect (toggle2, "toggled", G_CALLBACK (choose_format_opus), NULL);


//-----------------------VOLUME---BUTTONSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS



  volume1 = gtk_builder_get_object (builder, "volumebutton1");
  g_signal_connect (volume1, "value-changed",G_CALLBACK (change_volume1), NULL);



  volume2 = gtk_builder_get_object (builder, "volumebutton2");
  g_signal_connect (volume2, "value-changed",G_CALLBACK (change_volume2), NULL);

  volume3 = gtk_builder_get_object (builder, "volumebutton3");
  g_signal_connect (volume3, "value-changed",G_CALLBACK (change_volume3), NULL);

//-----------------------VOLUME---BUTTONSSSSSSSS-------END-------------------------


//--------------OPUS----SET--BIT-RATE----------------------------------------------------------------------------------

  entry7 = gtk_builder_get_object (builder, "entry7");
  opus_set_br = gtk_builder_get_object (builder, "button4");
    g_object_set_data(G_OBJECT(opus_set_br), "entry7", entry7);
  g_signal_connect (GTK_BUTTON(opus_set_br) , "clicked", G_CALLBACK (set_bit_rate), opus_set_br);


//--------------OPUS----SET--FRAME-SIZE----------------------------------------------------------------------------------

  entry6 = gtk_builder_get_object (builder, "entry6");
  opus_set_fs = gtk_builder_get_object (builder, "button6");
    g_object_set_data(G_OBJECT(opus_set_fs), "entry6", entry6);
  g_signal_connect (GTK_BUTTON(opus_set_fs) , "clicked", G_CALLBACK (set_frame_size), opus_set_fs);

//--------------OPUS----COMPLEXITY----------------------------------------------------------------------------------

  entry9 = gtk_builder_get_object (builder, "entry9");
  opus_set_compl = gtk_builder_get_object (builder, "button5");
    g_object_set_data(G_OBJECT(opus_set_compl), "entry9", entry9);
  g_signal_connect (GTK_BUTTON(opus_set_compl) , "clicked", G_CALLBACK (set_complexity), opus_set_compl);


//--------------OPUS----BANDWIDTH----------------------------------------------------------------------------------

  entry8 = gtk_builder_get_object (builder, "entry8");
  opus_bw = gtk_builder_get_object (builder, "button8");
    g_object_set_data(G_OBJECT(opus_bw), "entry8", entry8);
  g_signal_connect (GTK_BUTTON(opus_bw) , "clicked", G_CALLBACK (opus_bandwidth), opus_bw);

//--------------OPUS----CBR VBR CVBR----------------------------------------------------------------------------------

  entry11 = gtk_builder_get_object (builder, "entry11");
  opus_cbr = gtk_builder_get_object (builder, "button7");
    g_object_set_data(G_OBJECT(opus_cbr), "entry11", entry11);
  g_signal_connect (GTK_BUTTON(opus_cbr) , "clicked", G_CALLBACK (opus_cbr_vbr), opus_cbr);




//------------------ALSA----BUFFER SIZE INPUT----------------------------
  entry4 = gtk_builder_get_object (builder, "entry4");
  change_buf = gtk_builder_get_object (builder, "button2");
    g_object_set_data(G_OBJECT(change_buf), "entry4", entry4);
  g_signal_connect (GTK_BUTTON(change_buf) , "clicked", G_CALLBACK (change_buffer), change_buf);
//---------------------------------------------------------------------------------------------


//---------------ALSA-------sample rate----------------------------

  entry3 = gtk_builder_get_object (builder, "entry3");
  samp_rate = gtk_builder_get_object (builder, "button1");
    g_object_set_data(G_OBJECT(samp_rate), "entry3", entry3);
  g_signal_connect (GTK_BUTTON(samp_rate) , "clicked", G_CALLBACK (sample_rate), samp_rate);
//---------------------------------------------------------------------------------------------



//----------------------RECORDING----BUTTONS-------------------------------------

  button = gtk_builder_get_object (builder, "start_rec");
  g_signal_connect (button, "clicked", G_CALLBACK (start_rec), NULL);

  button = gtk_builder_get_object (builder, "stop_rec");
  g_signal_connect (button, "clicked", G_CALLBACK (stop_rec), NULL);

//----------------------PACKET SIZE INPUT----------------------------
  entry5 = gtk_builder_get_object (builder, "entry5");
  pac_sz = gtk_builder_get_object (builder, "button3");
    g_object_set_data(G_OBJECT(pac_sz), "entry5", entry5);
  g_signal_connect (GTK_BUTTON(pac_sz) , "clicked", G_CALLBACK (packet_size), pac_sz);








//---------------------------------------------------------------------------------------------

}

//-------------------------BUTTONS--------END-------------------------


int main (int   argc,   char *argv[])
{
  char command[50];
  pcm = 0;
  opus = 1;
  /* Initialize GTK */
  gtk_init (&argc, &argv);
  /* Initialize GStreamer */
  gst_init (&argc, &argv);


  
 

  //pipelines_pcm();

  //pipelines_pcm();
  //pipelines_opus();
  buttons();
  
  gtk_main ();

  return 0;
}


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
  
  toggle1 = gtk_builder_get_object (builder, "radiobutton1");
  g_signal_connect (toggle1, "toggled", G_CALLBACK (choose_format_pcm), NULL);

  toggle2 = gtk_builder_get_object (builder, "radiobutton2");
  g_signal_connect (toggle2, "toggled", G_CALLBACK (choose_format_opus), NULL);



//-----------------------VOLUME---BUTTONSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS



  volume1 = gtk_builder_get_object (builder, "volumebutton1");
  g_signal_connect (volume1, "value-changed",G_CALLBACK (change_volume1), NULL);



  volume2 = gtk_builder_get_object (builder, "volumebutton2");
  g_signal_connect (volume2, "value-changed",G_CALLBACK (change_volume2), NULL);



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


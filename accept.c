#include <gtk/gtk.h>

static void
accept_clicked (GtkButton *button, GObject *context_object)
{
    GtkLabel *accept_lable1 = g_object_get_data (context_object, "label1");
    GtkLabel *accept_lable2 = g_object_get_data (context_object, "label2");
    GtkEntry *accept_entry1 = g_object_get_data (context_object, "entry1");
    GtkEntry *accept_entry2 = g_object_get_data (context_object, "entry2");

    const char *entry1_in = gtk_entry_get_text (accept_entry1);
    gtk_label_set_text (accept_lable1, entry1_in);
    const char *entry2_in = gtk_entry_get_text (accept_entry2);
    gtk_label_set_text (accept_lable2, entry2_in);
}

int main( int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *entry1;
    GtkWidget *entry2;
    GtkWidget *buttonAccept;

    GObject *context_object;

    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 250);
    gtk_window_set_title(GTK_WINDOW(window), "Linux-buddy.blogspot.com");

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    //Create label
    label1 = gtk_label_new("label1");
    gtk_box_pack_start(GTK_BOX(box), label1, FALSE, FALSE, 3);
    label2 = gtk_label_new("label2");
    gtk_box_pack_start(GTK_BOX(box), label2, FALSE, FALSE, 3);

    //Create entryBox
    entry1 = gtk_entry_new ();
    gtk_box_pack_start(GTK_BOX(box), entry1, FALSE, FALSE, 3);
    entry2 = gtk_entry_new ();
    gtk_box_pack_start(GTK_BOX(box), entry2, FALSE, FALSE, 3);
    
    buttonAccept = gtk_button_new_with_label ("Accept");
    gtk_box_pack_start(GTK_BOX(box), buttonAccept, FALSE, FALSE, 3);
    g_object_set_data(G_OBJECT(buttonAccept), "label1", label1);
    g_object_set_data(G_OBJECT(buttonAccept), "label2", label2);
    g_object_set_data(G_OBJECT(buttonAccept), "entry1", entry1);
    g_object_set_data(G_OBJECT(buttonAccept), "entry2", entry2);
    g_signal_connect(GTK_BUTTON (buttonAccept), "clicked", 
        G_CALLBACK (accept_clicked), buttonAccept);

    //Connects GCallback function gtk_main_quit to "destroy" signal for "window"
    g_signal_connect(G_OBJECT(window), "destroy", 
        G_CALLBACK(gtk_main_quit), NULL);

    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}


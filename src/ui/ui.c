#include "ui.h"

Ui ui;


void start_process(void){
}

void aled(void){
}

int ui_start(int argc, char **argv){
    // Initialise GTK
    gtk_init(&argc, &argv);


    // Create window
    ui.pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // Set window title & display it
    gtk_window_set_title(GTK_WINDOW(ui.pWindow),
        "Hey Morty!");
    gtk_window_set_decorated(GTK_WINDOW(ui.pWindow), TRUE);

    // Center the window
    gtk_window_set_position(GTK_WINDOW(ui.pWindow), GTK_WIN_POS_CENTER);

    // Set window size
    gtk_window_set_default_size(GTK_WINDOW(ui.pWindow), 320, 200);

    // User can resize the window
    gtk_window_set_resizable(GTK_WINDOW(ui.pWindow), TRUE);


    // Set text
    ui.pLabel = gtk_label_new(NULL);
    ui.text = "<span>Hey I'm Morty, your personal voice assistant!\n\
How can I help you?</span>";
    gtk_label_set_markup(GTK_LABEL(ui.pLabel), ui.text);
    gtk_label_set_justify(GTK_LABEL(ui.pLabel), GTK_JUSTIFY_CENTER);

    // Create button to launch the voice recognition
    ui.Voice_button = gtk_button_new_with_label("Start voice recignition");
    ui.Help_button = gtk_button_new_with_label("Help");
    

    // Create box
    ui.Box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    ui.IBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // Add to Box
    gtk_box_pack_start(GTK_BOX(ui.Box), ui.pLabel, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(ui.Box), ui.Voice_button, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(ui.Box), ui.IBox, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(ui.Box), ui.Help_button, FALSE, FALSE, 0);

    // Add Box to window
    gtk_container_add(GTK_CONTAINER(ui.pWindow), ui.Box);

    // show the window
    gtk_widget_show_all(ui.pWindow);

    // signal connection
    g_signal_connect(G_OBJECT(ui.pWindow), "destroy", G_CALLBACK(gtk_main_quit),
        NULL);
    g_signal_connect(ui.Voice_button, "clicked", G_CALLBACK(start_process),
        ui.IBox);
    g_signal_connect(ui.Help_button, "clicked", G_CALLBACK(aled),
        NULL);

    //Runs the main loop
    gtk_main();

    return EXIT_SUCCESS;
}

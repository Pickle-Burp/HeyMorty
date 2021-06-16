#include "ui.h"

Ui ui;

void homepage(void){
    gtk_widget_destroy(ui.BoxHelp);

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
    ui.text = "<span>Salut je suis Morty, votre assistant vocal personnel !\n\
Comment puis-je vous aider ?</span>";
    gtk_label_set_markup(GTK_LABEL(ui.pLabel), ui.text);
    gtk_label_set_justify(GTK_LABEL(ui.pLabel), GTK_JUSTIFY_CENTER);

    // Create button to launch the voice recognition
    ui.Voice_button = gtk_button_new_with_label("Lancer la reconnaissance vocale");
    ui.Help_button = gtk_button_new_with_label("Aide");


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
                     NULL);
    g_signal_connect(ui.Help_button, "clicked", G_CALLBACK(aled),
                     NULL);

    //Runs the main loop
    gtk_main();
}

void start_process(void){
}

void aled(void){
    gtk_widget_destroy(ui.Box);
    // Set text
    ui.pLabelHelp = gtk_label_new(NULL);
    ui.textHelp = "<span>Bienvenue dans le menu d'aide.\n\
Ici vous trouverez toutes les actions que je peux realiser \n\n\n\
* Ouvrir tout type d'application presente sur l'ordinateur \n\
    Demandez moi par exemple : \"ouvre l'application firefox\"\n\
* Realiser une recherche sur internet \n\
    Vous pouvez me demander cela de 2 manieres : \n\
        * En precisant le navigateur : \n\
            Recherche sur/dans [navigateur] [votre recherche] \n\
            Recherche [votre recherche] sur/dans [navigateur] \n\
        * En utilisant votre navigateur par defaut : \n\
            Recherche [votre recherche] \n</span>";

    gtk_label_set_markup(GTK_LABEL(ui.pLabelHelp), ui.textHelp);
    gtk_label_set_justify(GTK_LABEL(ui.pLabel), GTK_JUSTIFY_CENTER);

    ui.Return_button = gtk_button_new_with_label("Retour");
    ui.IBoxHelp = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    ui.BoxHelp = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(ui.BoxHelp), ui.pLabelHelp, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(ui.BoxHelp), ui.IBoxHelp, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(ui.BoxHelp), ui.Return_button, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(ui.pWindow), ui.BoxHelp);
    gtk_widget_show_all(ui.pWindow);

    g_signal_connect(ui.Return_button, "clicked", G_CALLBACK(homepage), NULL);
}

int ui_start(int argc, char **argv){
    // Initialise GTK
    gtk_init(&argc, &argv);

    // Create window
    ui.pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    homepage();

    return EXIT_SUCCESS;
}

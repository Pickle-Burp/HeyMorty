#include "ui.h"

Ui ui;

/**
 * Set the homepage of the user interface
 * @author CHEVREAU Annabelle
 */
void homepage(void){
    gtk_widget_destroy(ui.BoxHelp);

    // Set window title & display it
    gtk_window_set_title(GTK_WINDOW(ui.pWindow),
                         "Hey Morty!");
    gtk_window_set_decorated(GTK_WINDOW(ui.pWindow), TRUE);

    // Center the window
    gtk_window_set_position(GTK_WINDOW(ui.pWindow),
                            GTK_WIN_POS_CENTER);

    // Set window size
    gtk_window_set_default_size(GTK_WINDOW(ui.pWindow), 320,
                                200);

    // User can resize the window
    gtk_window_set_resizable(GTK_WINDOW(ui.pWindow), TRUE);

    // Set text
    ui.pLabel = gtk_label_new(NULL);
    ui.text = "<span>\n\n\n\nSalut je suis Morty, \
votre assistant vocal personnel !\n\
Comment puis-je vous aider ?\n\n\n</span>";
    gtk_label_set_markup(GTK_LABEL(ui.pLabel), ui.text);
    gtk_label_set_justify(GTK_LABEL(ui.pLabel), GTK_JUSTIFY_CENTER);

    // Create button to launch the voice recognition
    ui.Voice_button = gtk_button_new_with_label("Lancer la reconnaissance"
                                                " vocale");
    ui.Help_button = gtk_button_new_with_label("Aide");


    // Create box
    ui.Box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    ui.IBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // Add to Box
    gtk_box_pack_start(GTK_BOX(ui.Box), ui.pLabel,
                       FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(ui.Box), ui.Voice_button,
                       FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(ui.Box), ui.IBox,
                       TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(ui.Box), ui.Help_button,
                       FALSE, FALSE, 0);

    // Add Box to window
    gtk_container_add(GTK_CONTAINER(ui.pWindow), ui.Box);
    // show the window
    gtk_widget_show_all(ui.pWindow);

    // signal connection
    g_signal_connect(G_OBJECT(ui.pWindow),
                     "destroy", G_CALLBACK(gtk_main_quit),
                     NULL);
    g_signal_connect(ui.Voice_button,
                     "clicked", G_CALLBACK(start_process),
                     NULL);
    g_signal_connect(ui.Help_button,
                     "clicked", G_CALLBACK(aled),
                     NULL);

    //Runs the main loop
    gtk_main();
}

/**
 * Executes the main program
 * @author CHEVREAU Annabelle
 */
void start_process(void){

}

/**
 * Set help menu
 * @author CHEVREAU Annabelle
 */
void aled(void){
    gtk_widget_destroy(ui.Box);
    // Set text
    ui.pLabelHelp = gtk_label_new(NULL);
    ui.textHelp = "<span>\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nBienvenue dans le \
menu d'aide.\n\
Ici vous trouverez toutes les actions que je peux realiser \n\
Les étapes entre crochets sont à remplacer par ce qui est indiqué a \
l'intérieur. \n\
Les étapes entre parenthèses sont facultatives. \n\n\n\
* Ouvrir tout type d'application presente sur l'ordinateur \n\
    Demandez le moi sous la forme : Ouvre (l'application) \
[nom de l'application]\n\
* Realiser une recherche sur internet \n\
    Vous pouvez me demander cela de 2 manieres : \n\
        * En precisant le navigateur : \n\
            Recherche sur/dans [navigateur] [votre recherche] \n\
            Recherche [votre recherche] sur/dans [navigateur] \n\
        * En utilisant votre navigateur par defaut : \n\
            Recherche [votre recherche] \n\
* Trouver tout type de dossiers presents sur l'ordinateur \n\
    Demandez le moi sous la forme : Trouve le dossier [nom du dossier] \n\
* Trouver tout type de fichiers presents sur l'ordinateur \n\
    Demandez le moi sous la forme : Trouve le fichier "
                  "[nom du fichier.extension]\n\n\n</span>";

    gtk_label_set_markup(GTK_LABEL(ui.pLabelHelp), ui.textHelp);
    gtk_label_set_justify(GTK_LABEL(ui.pLabel), GTK_JUSTIFY_CENTER);

    ui.Return_button = gtk_button_new_with_label("Retour");
    ui.IBoxHelp = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    ui.BoxHelp = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(ui.BoxHelp), ui.pLabelHelp,
                       FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(ui.BoxHelp), ui.IBoxHelp,
                       TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(ui.BoxHelp), ui.Return_button,
                       FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(ui.pWindow), ui.BoxHelp);
    gtk_widget_show_all(ui.pWindow);

    g_signal_connect(ui.Return_button, "clicked",
                     G_CALLBACK(homepage), NULL);
}

/**
 * User interface initilisation
 * @author CHEVREAU Annabelle
 */
int ui_start(int argc, char **argv){
    // Initialise GTK
    gtk_init(&argc, &argv);

    // Create window
    ui.pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    homepage();

    return EXIT_SUCCESS;
}

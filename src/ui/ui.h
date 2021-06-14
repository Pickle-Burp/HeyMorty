#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtk/gtk.h>
#include <stdlib.h>
#include <glib/gi18n.h>


typedef struct Ui{
    GtkWidget *pWindow;
    GtkWidget *pWindowHelp;
    GtkWidget *pLabel;
    GtkWidget *pLabelHelp;
    GtkWidget *Box;
    GtkWidget *BoxHelp;
    GtkWidget *IBox;
    GtkWidget *IBoxHelp;
    GtkWidget *Voice_button;
    GtkWidget *Help_button;
    GtkWidget *Return_button;
    GtkWidget *image;
    gchar *text;
    gchar *textHelp;
    gchar* filename;
    char* text_ocr;
} Ui;

void start_process(void);
void aled(void);
int main(int argc, char **argv);

#endif

#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtk/gtk.h>
#include <stdlib.h>
#include <glib/gi18n.h>

#include "../main.h"

typedef struct Ui{
    GtkWidget *pWindow;
    GtkWidget *pLabel;
    GtkWidget *pLabel2;
    GtkWidget *Box;
    GtkWidget *IBox;
    GtkWidget *Open_button;
    GtkWidget *Start_button;
    GtkWidget *Download_button;
    GtkWidget *image;
    gchar *text;
    gchar *text2;
    gchar* filename;
    char* text_ocr;
} Ui;

static void select_image(void);
void start(void);
void download_doc(void);
int main(int argc, char **argv);

#endif

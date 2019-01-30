/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: void
 *
 * Created on 18 декабря 2018 г., 16:51
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <gtk-3.0/gtk/gtk.h>
#include <glib-2.0/gio/gfile.h>
#include <glib-2.0/gio/giotypes.h>
#include <glib-2.0/gio/gioenums.h>
#include <glib-2.0/gio/goutputstream.h>
#include <glib-2.0/gio/ginputstream.h>


GtkWindow *mainWin;
GtkApplication *mainApp;
GtkWidget *fontEdit, *fontView, *fontBuf;
GtkSpinButton *spin, *limt;
GtkLabel *fileLab;
uint8_t *dataBuf = NULL;
gchar fname[1024];
uint8_t copyBuf[8];
/*
 * 
 */
void menu_quit_cb(GtkMenuItem *i, gpointer p) {
    g_application_quit(G_APPLICATION(mainApp));
}
static uint8_t save_file() {
    gsize saved;
    gint lim = gtk_spin_button_get_value_as_int(limt);
    GFile *fl = g_file_new_for_path(fname);
    GFileOutputStream *os = g_file_replace(fl, NULL, FALSE, G_FILE_CREATE_NONE, NULL, NULL);
    g_output_stream_write_all((GOutputStream*)os, dataBuf, lim * 8, &saved, NULL, NULL);
    g_output_stream_close((GOutputStream*)os, NULL, NULL);
    g_object_unref(os);
    g_object_unref(fl);
    return(TRUE);
}

static uint8_t read_file() {
    GFile *fl = g_file_new_for_path(fname);
    GFileInputStream *is = g_file_read(fl, NULL, NULL);// g_file_replace(fl, NULL, FALSE, G_FILE_CREATE_NONE, NULL, NULL);
    gssize sz = g_input_stream_read((GInputStream*)is, dataBuf, 2048, NULL, NULL);
    g_input_stream_close((GInputStream*)is, NULL, NULL);
    g_object_unref(is);
    g_object_unref(fl);
    if (sz < 0) return(FALSE);
    sz = sz / 8;
    gtk_spin_button_set_value(limt, (double) sz);
    return(TRUE);
}
void menu_save_cb() {
    save_file();
}
void menu_saveas_cb() {
    GtkFileFilter *ff = gtk_file_filter_new();
    gtk_file_filter_set_name(ff,"Font Master");
    gtk_file_filter_add_pattern(ff,"*.fnt");
    GtkWidget *fc = gtk_file_chooser_dialog_new("Save File As", mainWin, GTK_FILE_CHOOSER_ACTION_SAVE,
            GTK_STOCK_CLOSE, GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE_AS, GTK_RESPONSE_ACCEPT, NULL);
    GtkFileChooser *choo = GTK_FILE_CHOOSER(fc);
    gtk_file_chooser_add_filter(choo,ff);
    gtk_file_chooser_set_do_overwrite_confirmation(choo, TRUE);
    gtk_file_chooser_set_filename(choo, fname);
    gint res = gtk_dialog_run(GTK_DIALOG(fc));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *fn = gtk_file_chooser_get_filename(choo);
        g_stpcpy(fname, fn);
        g_free(fn);
        gtk_label_set_text(fileLab,fname);
        save_file();
    }
    gtk_widget_destroy(fc);
}

void menu_open_cb() {
    GtkFileFilter *ff = gtk_file_filter_new();
    gtk_file_filter_set_name(ff,"Font Master");
    gtk_file_filter_add_pattern(ff,"*.fnt");
    GtkWidget *fc = gtk_file_chooser_dialog_new("Open file", mainWin, GTK_FILE_CHOOSER_ACTION_OPEN,
            GTK_STOCK_CLOSE, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
    GtkFileChooser *choo = GTK_FILE_CHOOSER(fc);
    gtk_file_chooser_add_filter(choo,ff);
    gint res = gtk_dialog_run(GTK_DIALOG(fc));
    if (res == GTK_RESPONSE_ACCEPT) {
        char *fn = gtk_file_chooser_get_filename(choo);
        g_stpcpy(fname, fn);
        g_free(fn);
        gtk_label_set_text(fileLab,fname);
        read_file();
        gtk_widget_queue_draw(GTK_WIDGET(mainWin));
    }
    gtk_widget_destroy(fc);
    //g_object_unref(ff);  // это не надо чусер сам удаляет фильтры
}

void menu_new_cb() {
    for(uint16_t i1 = 0;i1 < (8*256); i1++) dataBuf[i1] = 0;
    g_stpcpy(fname,"unnamed.fnt");    
    gtk_label_set_text(fileLab,fname);
    gtk_widget_queue_draw(GTK_WIDGET(mainWin));
    gtk_spin_button_set_value(limt, (double) 256);
}
void app_init(GApplication *app) {
    GtkBuilder *bld = gtk_builder_new_from_file("/usr/share/fmaster/fontmas.glade");
    if (bld) {
        mainWin = (GtkWindow*) gtk_builder_get_object(bld,"mainwin");
        fontEdit = (GtkWidget*) gtk_builder_get_object(bld,"fontedit");
        fontView = (GtkWidget*) gtk_builder_get_object(bld,"fontview");
        fontBuf = (GtkWidget*) gtk_builder_get_object(bld,"fontbuf");
        spin = (GtkSpinButton*) gtk_builder_get_object(bld,"spinbut1");
        limt = (GtkSpinButton*) gtk_builder_get_object(bld,"spinbut2");
        fileLab = (GtkLabel*) gtk_builder_get_object(bld,"filelab");
        gtk_widget_add_events(fontEdit,GDK_BUTTON_PRESS_MASK);
        gtk_widget_add_events(fontView,GDK_BUTTON_PRESS_MASK);
        gtk_application_add_window(mainApp,mainWin);
        gtk_builder_connect_signals(bld,NULL);
        gtk_widget_show_all(GTK_WIDGET(mainWin));
        g_object_unref(bld);
        dataBuf = g_malloc(8 * 256);
        menu_new_cb();
        dataBuf[0] = 4;
        dataBuf[1] = 10;
        dataBuf[2] = 17;
        dataBuf[3] = 31;
        dataBuf[4] = 17;
        dataBuf[5] = 17;
        dataBuf[6] = 17;
        dataBuf[7] = 0;
        dataBuf[8] = 4;
        dataBuf[9] = 10;
        dataBuf[10] = 17;
        dataBuf[11] = 31;
        dataBuf[12] = 17;
        dataBuf[13] = 17;
        dataBuf[14] = 17;
        dataBuf[15] = 0;
//        g_application_quit(G_APPLICATION(mainApp));
//        if (!open_dev()) g_application_quit(G_APPLICATION(mainApp));
    }
}

int main(int argc, char** argv) {
    mainApp = gtk_application_new("com.svahard.fmaster",G_APPLICATION_FLAGS_NONE);
    g_signal_connect(G_APPLICATION(mainApp),"activate",G_CALLBACK(app_init),mainApp);
    gint stat = g_application_run(G_APPLICATION(mainApp),argc,argv);
    g_free(dataBuf);
    g_object_unref(mainApp);
    return (EXIT_SUCCESS);
}


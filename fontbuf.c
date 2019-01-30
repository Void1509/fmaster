/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdint.h>
#include <stdio.h>
#include <gtk-3.0/gtk/gtk.h>
#include "fontmas.h"

gboolean copy_draw_cb(GtkWidget *wid, cairo_t *ct, gpointer data) {
    GtkStyleContext *ctx = gtk_widget_get_style_context(wid);
    gint x = gtk_widget_get_allocated_width(wid);
    gint y = gtk_widget_get_allocated_height(wid);
    gint ch1 = gtk_spin_button_get_value_as_int(spin);
//    printf("%d\n",ch1);
//    cairo_set_source_rgb(ct, 0, 0, 0.2);
//    cairo_rectangle(ct, 0, 0, x - 1, y - 1);
//    cairo_fill(ct);
    gtk_render_background(ctx, ct, 0, 0, x, y);
    for(uint8_t y1 = 0; y1 < 8; y1++) {
        uint8_t ln = copyBuf[y1];
        uint8_t mk = 1 << 5;
        for(uint8_t x1 = 0; x1 < 6; x1++) {
            if (ln & mk) drect(ct, 5 + (x1 * rsize), 5 + (y1 * rsize), 1); else drect(ct, 5 + (x1 * rsize), 5 + (y1 * rsize), 0);
            mk >>= 1;
        }
    }
    return(TRUE);
}


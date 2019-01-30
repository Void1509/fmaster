/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdint.h>
#include <gtk-3.0/gtk/gtk.h>
#include <cairo/cairo.h>
#include <gtk-3.0/gdk/gdkevents.h>
#include <gtk-3.0/gtk/gtkspinbutton.h>
#include <gtk-3.0/gtk/gtkenums.h>
#include <gtk-3.0/gtk/gtkwidget.h>
#include "fontmas.h"


static const struct {
    double r,g,b;
} dcol[3] = {{0, 0, 0.2}, {0, 0.8, 0.5}, {1, 1, 0.5}};

void drect(cairo_t *c, uint16_t x, uint16_t y, uint8_t cl) {
    cl &= 1;
    cairo_set_source_rgb(c, dcol[cl].r, dcol[cl].g, dcol[cl].b);
    cairo_rectangle(c, x, y, rsize, rsize);
    cairo_fill(c);
    cairo_set_source_rgb(c, dcol[2].r, dcol[2].g, dcol[2].b);
    cairo_rectangle(c, x, y, rsize, rsize);
    cairo_stroke(c);
}
void spin_change_cb(GtkWidget *w, gpointer p) {
    gint ch1 = gtk_spin_button_get_value_as_int(spin);
    set_cursor(ch1 % 16, ch1 / 16);
    gtk_widget_queue_draw(fontEdit);
}
gboolean but_press_cb(GtkWidget *dw, GdkEventButton *ev, gpointer p) {
    if (ev->type == GDK_BUTTON_PRESS) {
        gint x = ev->x;
        gint y = ev->y;
        if ((x < 5) || (y < 5)) return(TRUE);
        gint ch1 = gtk_spin_button_get_value_as_int(spin);
        x -= 5;
        y -= 5;
        x = x / rsize;
        y = y / rsize;
        if ((x > 5) || (y > 7)) return(TRUE);
        uint8_t mk = 32 >> x;
        dataBuf[(ch1*8)+y] ^= mk;
        gtk_widget_queue_draw(fontEdit);
        gtk_widget_queue_draw(fontView);
        return(TRUE);
    }
    return(FALSE);
}

gboolean edit_draw_cb(GtkWidget *wid, cairo_t *ct, gpointer data) {
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
        uint8_t ln = dataBuf[(ch1*8)+y1];
        uint8_t mk = 1 << 5;
        for(uint8_t x1 = 0; x1 < 6; x1++) {
            if (ln & mk) drect(ct, 5 + (x1 * rsize), 5 + (y1 * rsize), 1); else drect(ct, 5 + (x1 * rsize), 5 + (y1 * rsize), 0);
            mk >>= 1;
        }
    }
    return(TRUE);
}
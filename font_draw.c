/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdint.h>
#include <gtk-3.0/gtk/gtk.h>
#include <cairo/cairo.h>
#include <string.h>
#include "fontmas.h"
#define BL_SIZE 3
#define wsize   24
#define hsize   32

static struct {
    uint8_t x, y;
} curs = {0.0};

static const struct {
    double r, g, b;
} fcol[3] = {
    {0, 0, 0.2},
    {0.2, 0.8, 0},
    {1, 1, 0.5}
};

void set_cursor(gint x, gint y) {
    curs.x = x;
    curs.y = y;
    gtk_widget_queue_draw(fontView);
}

static void draw_char(cairo_t *c, uint16_t x, uint16_t y, uint8_t ch, uint8_t sel) {
    cairo_set_source_rgb(c, fcol[1].r, fcol[1].g, fcol[1].b);
    if (sel) {
        for (uint8_t y1 = 0; y1 < 8; y1++) {
            uint8_t ln = dataBuf[(ch * 8) + y1];
            uint8_t mk = 1 << 5;
            for (uint8_t x1 = 0; x1 < 6; x1++) {
                if (!(ln & mk)) cairo_rectangle(c, x + (x1 * BL_SIZE), y + (y1 * BL_SIZE), BL_SIZE, BL_SIZE);
                mk >>= 1;
            }
        }
    } else {
        for (uint8_t y1 = 0; y1 < 8; y1++) {
            uint8_t ln = dataBuf[(ch * 8) + y1];
            uint8_t mk = 1 << 5;
            for (uint8_t x1 = 0; x1 < 6; x1++) {
                if (ln & mk) cairo_rectangle(c, x + (x1 * BL_SIZE), y + (y1 * BL_SIZE), BL_SIZE, BL_SIZE);
                mk >>= 1;   
            } 
        }
    }
    cairo_fill(c);
}

gboolean select_cb(GtkWidget *dw, GdkEventButton *ev, gpointer p) {
    //    printf("%2x:%2x\n", ev->button, ev->state);
    gint x = ev->x;
    gint y = ev->y;
    x = x / wsize;
    y = y / hsize;
    if ((x > 15) || (y > 15)) return (TRUE);

    if (ev->button == 1) {
        if (ev->type == GDK_BUTTON_PRESS) {
            curs.x = x;
            curs.y = y;
            gint i1 = (y * 16) + x;
            gtk_spin_button_set_value(spin, (double) i1);
            gtk_widget_queue_draw(fontView);
            gtk_widget_queue_draw(fontEdit);
            return (TRUE);
        }
        if (ev->type == GDK_2BUTTON_PRESS) {
            //            puts("Double click");
            gint ch1 = gtk_spin_button_get_value_as_int(spin);
            memcpy(copyBuf, &dataBuf[ch1 * 8], 8);
            gtk_widget_queue_draw(fontBuf);
        }
    }
    if (ev->button == 3) {
        if (ev->type == GDK_BUTTON_PRESS) {
            gint ch1 = gtk_spin_button_get_value_as_int(spin);
            memcpy(&dataBuf[ch1 * 8], copyBuf, 8);
            gtk_widget_queue_draw(fontView);
            gtk_widget_queue_draw(fontEdit);
        }
        //        if (ev->type == GDK_2BUTTON_PRESS) {
        //            puts("Double click");
        //            gint i1 = (y * 16) + x;
        //            gtk_spin_button_set_value(spin, (double) i1);
        //            gtk_widget_queue_draw(fontEdit);
        //        }
    }

    return (FALSE);
}
void limt_change_cb(GtkWidget *w, gpointer p) {
    gtk_widget_queue_draw(fontView);
}
gboolean font_draw_cb(GtkWidget *wid, cairo_t *ct, gpointer data) {
    GtkStyleContext *ctx = gtk_widget_get_style_context(wid);
    gint x = gtk_widget_get_allocated_width(wid);
    gint y = gtk_widget_get_allocated_height(wid);
    //   uint8_t chr = 0;
    gint ch1 = gtk_spin_button_get_value_as_int(limt);

    gtk_render_background(ctx, ct, 0, 0, x, y);
    cairo_set_antialias(ct, CAIRO_ANTIALIAS_NONE);
    cairo_set_line_width(ct, 1.0);
    //    cairo_set_source_rgb(ct, 0, 0, 0.2);
    //    cairo_rectangle(ct, 0, 0, x - 1, y - 1);
    //    cairo_fill(ct);


    for (uint16_t chr = 0; chr < ch1; chr++) {
        uint16_t x = chr % 16;
        uint16_t y = chr / 16;
        cairo_set_source_rgb(ct, 0, 0, 0.2);
        cairo_rectangle(ct, x * wsize, y * hsize, wsize, hsize);
        cairo_fill(ct);

        if ((x == curs.x) && (y == curs.y)) {
            cairo_set_source_rgb(ct, fcol[2].r, fcol[2].g, fcol[2].b);
            cairo_rectangle(ct, (x * wsize), (y * hsize), wsize, hsize);
            cairo_stroke(ct);
        }
        draw_char(ct, x * wsize + 2, y * hsize + 4, chr, 0);
    }
    return (TRUE);
}
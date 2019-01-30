/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fontmas.h
 * Author: void
 *
 * Created on 20 декабря 2018 г., 15:11
 */

#ifndef FONTMAS_H
#define FONTMAS_H

#define rsize   20

extern GtkWindow *mainWin;
extern GtkApplication *mainApp;
extern GtkWidget *fontEdit, *fontView, *fontBuf;
extern GtkSpinButton *spin, *limt;
extern GtkLabel *fileLab;
extern uint8_t *dataBuf;
extern gchar fname[1024];
extern uint8_t copyBuf[8];

void drect(cairo_t *c, uint16_t x, uint16_t y, uint8_t cl);
void set_cursor(gint x, gint y);
#endif /* FONTMAS_H */


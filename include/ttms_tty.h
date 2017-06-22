/***********************************************************************
    Ticket Theater Management System
    Copyright(C) 2017 hepangda

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    3 any later version.

    This program is distributed in the hope that it will be useful,
    buf WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY of FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have recived a copy of the GNU Gereral Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

    Author: hepangda
    E-mail: pangda@xiyoulinux.org
*************************************************************************/

#ifndef _TTMS_TTY_H
#define _TTMS_TTY_H

#include<stdio.h>
#include<sys/termios.h>
#include<termios.h>


#define TTY_ERR_GETWINSIZE (GLOBAL_TTY_ERRNO + 1)

static enum {
     RESET, RAW, CBREAK
} ttystate = RESET;
static struct termios save_termios;
static int ttysavefd = -1;

int tty_disp(int);
int tty_cbreak();
int tty_raw();
int tty_reset();
void tty_atexit();
struct termios *tty_termios();
struct winsize tty_winsize();
#define tty_cursor_gotoxy(y, x) \
    fprintf(stderr, "\033[%d;%dH", (y), (x))
#define tty_cursor_up(n) \
    fprintf(stderr, "\033[%dA", (n))
#define tty_cursor_down(n) \
    fprintf(stderr, "\033[%dB", (n))
#define tty_cursor_left(n) \
    fprintf(stderr, "\033[%dC", (n))
#define tty_cursor_right(n) \
    fprintf(stderr, "\033[%dD", (n))
#define tty_cursor_hide fprintf(stderr, "\033[?25l")
#define tty_cursor_show fprintf(stderr, "\033[?25h")
#define tty_closeattr fprintf(stderr, "\033[0m")
#define tty_highlight fprintf(stderr, "\033[1m")
#define tty_underline fprintf(stderr, "\033[4m")
#define tty_flash fprintf(stderr, "\033[5m")
#define tty_invert fprintf(stderr, "\033[7m")
#define tty_transparent fprintf(stderr, "\033[8m")
#define tty_color(color) \
    fprintf(stderr, "\033[%dm", (color))
#define tty_clearscreen fprintf(stderr, "\033[2J")
#define tty_clearline fprintf(stderr, "\033[K")

#endif

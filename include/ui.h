/***********************************************************************
    Theater Ticket Management System
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
#ifndef _TTMS_UI_H
#define _TTMS_UI_H

#include"global.h"
#include<stdio.h>

#define ui_write(...) fprintf(stderr, ##__VA_ARGS__)
#define ui_write_widthstring(...) fprintf(stderr, "%-*s", ##__VA_ARGS__)
#define ui_write_string(...) fprintf(stderr, "%-s", ##__VA_ARGS__)
#define ui_draw(y, x, ...) fprintf(stderr, "\033[%d;%dH", (y), (x)); fprintf(stderr, ##__VA_ARGS__);
#define ui_draw_highlight(y, x, ...) fprintf(stderr, "\033[%d;%dH\033[1m", (y), (x)); fprintf(stderr, ##__VA_ARGS__); fprintf(stderr, "\033[0m");
#define ui_draw_item(y, x, ...) fprintf(stderr, "\033[%d;%dH\033[1m\033[4m", (y), (x)); fprintf(stderr, ##__VA_ARGS__); fprintf(stderr, "\033[0m");



#define ui_clearline(n) tty_cursor_gotoxy((n), 0); ui_write_widthstring(WINSZ.ws_col, "");
#define ui_clearlines(from, to) for (int UICLEARLINESVAR = (from); UICLEARLINESVAR <= (to); UICLEARLINESVAR++) { \
                                ui_clearline(UICLEARLINESVAR); }
#define ui_scanf(input, ...) ui_getstring(input); sscanf(input, ##__VA_ARGS__);


#define UI_ITEM_PERPAGE (15)

typedef struct tag_ui_pos {
    int x, y;
} ui_pos_t;

int ui_initilize();
int ui_exit();


#define UI_RET_PASSWORDOVER (1)
int ui_login_getpasswd();
int ui_draw_login();
int ui_login();


int ui_draw_main_menu();
int ui_main_menu();

int ui_draw_studio(int, int);
int ui_draw_studio_update();
int ui_draw_studio_delete();
int ui_draw_studio_add();
int ui_studio();
int ui_studio_update();
int ui_studio_delete();
int ui_studio_add();

int ui_draw_seat(int);
int ui_draw_seat_add();
int ui_draw_seat_delete();
int ui_draw_seat_update();
int ui_draw_seat_ask();
int ui_seat();
int ui_seat_add();
int ui_seat_delete();
int ui_seat_update();
int ui_seat_ask();

int ui_draw_account(int, int);
int ui_draw_account_add();
int ui_draw_account_delete();
int ui_draw_account_update();
int ui_draw_account_find();
int ui_draw_account_lowpower(int);
int ui_draw_account_changepwd();
int ui_account();
int ui_account_add();
int ui_account_delete();
int ui_account_update();
int ui_account_find();
int ui_account_lowpower();
int ui_account_changepwd();

int ui_draw_play(int, int);
int ui_draw_play_add();
int ui_draw_play_delete();
int ui_draw_play_update();
int ui_play();
int ui_play_add();
int ui_play_delete();
int ui_play_update();

int ui_draw_query(int, int);
int ui_draw_query_sa();
int ui_query();
int ui_query_update();
int ui_query_sa();

int ui_draw_sale(int, int);
int ui_sale_schedule();
int ui_draw_sale_sale();
int ui_draw_sale_ret();
int ui_build_schedule();
int ui_build_ticket();
int ui_sale();
int ui_sale_sale();
int ui_sale_ticket();
int ui_sale_ask();
int ui_sale_ret();


int ui_draw_schedule(int, int);
int ui_draw_schedule_add();
int ui_draw_schedule_delete();
int ui_draw_schedule_update();
int ui_schedule();
int ui_schedule_add();
int ui_schedule_delete();
int ui_schedule_update();

#endif

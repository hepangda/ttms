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
#include"include/ttms_datastruct.h"
#include"include/ttms_ui_func.h"
#include"include/ttms_stddef.h"
#include"include/ttms_tty.h"
#include"include/ttms_ui.h"
#include<sys/ioctl.h>
#include<unistd.h>
#include<string.h>

extern struct winsize WINSZ;
extern int (*next_ui)();
int ui_getstring(char *dest) {
    tty_cursor_show;
    tty_reset();
    scanf("%s", dest);
    tty_cursor_hide;
    tty_cbreak();
    return RET_SUCCEED;
}

int ui_pager_maxpage(dstruct_linklist list) {
    dstruct_linklist_link plink = list.dll_phead->next;

    int end = 0, i, j;
    for (i = 0; ; i++) {
        for (j = 0; j < UI_ITEM_PERPAGE; j++) {
            if (plink == NULL) {
                end = 1;
                break;
            } else {
                plink = plink->next;
            }
        }
        if (end) {
            break;
        }
    }
    return i;
}

dstruct_linklist_link
ui_pager(dstruct_linklist list, int pages) {
    dstruct_linklist_link plink = list.dll_phead->next,
        ret = plink;

    int end = 0;
    for (int i = 0; i < pages; i++) {
        ret = plink;
        for (int j = 0; j < UI_ITEM_PERPAGE; j++) {
            if (plink == NULL) {
                end = 1;
                break;
            } else {
                plink = plink->next;
            }
        }
        if (end) {
            break;
        }
    }
    return ret;
}

int ui_title(const char *title) {
    tty_cursor_gotoxy(0, 0);
    tty_invert;
    ui_write("%-*s", WINSZ.ws_col, title);
    tty_closeattr;
    return RET_SUCCEED;
}

int ui_bottom(const char *bottom) {;
    tty_cursor_gotoxy(WINSZ.ws_row, 0);
    tty_invert;
    ui_write("%-*s", WINSZ.ws_col, bottom);
    tty_closeattr;
    return RET_SUCCEED;
}

int ui_window(const char *title) {
    ui_clearlines(0, WINSZ.ws_row);
    ui_title(title);
    return RET_SUCCEED;
}

int ui_flush() {
    char t;
    while ((t = getchar()) != '\n');
    return RET_SUCCEED;
}

int ui_request(const char *sign) {
    char t;
    ui_bottom(sign);
    read(STDIN_FILENO, &t, 1);
    t &= 255;
    return t;
}

int ui_powerfailed(int (*last_ui)()) {
    ui_request("\033[47;31mYou don\'t have power to do this! Press any key to continue..\033[0m");
    next_ui = last_ui;
}

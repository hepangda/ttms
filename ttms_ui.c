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
#include"include/ttms_ui.h"
#include"include/ttms_tty.h"
#include"include/ttms_ui_func.h"
#include"include/ttms_global.h"
#include"include/ttms_srv.h"
#include<unistd.h>
#include<sys/ioctl.h>
#include<string.h>

int (*next_ui)();
struct winsize WINSZ;

int ui_initilize() {
    tty_cbreak();
    tty_cursor_hide;
    WINSZ = tty_winsize();
    return RET_SUCCEED;
}

int ui_exit() {
    ui_clearlines(0, WINSZ.ws_row);
    tty_reset();
    tty_cursor_show;
    WATCHDOG = 0;
    return RET_SUCCEED;
}

int ui_login_getpasswd(char *dest) {
    tty_reset();
    tty_disp(0);
    tty_cursor_show;
    int cnt = 0;
    char t;
    ui_flush();
    do {
        t = getchar();
        if (t != '\n' || t != '\r') {
            dest[cnt++] = t;
        }
        if (cnt >= DEF_PASSWD_LENGTH) {
            return UI_RET_PASSWORDOVER;
        }
    } while (t != '\n' && t != '\r');
    dest[cnt - 1] = '\0';
    tty_cursor_hide;
    tty_disp(1);
    tty_cbreak();
    return RET_SUCCEED;
}

int ui_draw_login() {
    ui_window("TTMS Login");
    static const ui_pos_t selections[] = { {5, 40}, {6, 40} };
    static const char *slt_string[] = { "Username: ", "Password: " };

    for (int i = 0; i < 2; i++) {
        ui_draw(selections[i].x, selections[i].y, slt_string[i]);
    }

    return RET_SUCCEED;
}

int ui_login() {
    user_t loginmsg;
    static const ui_pos_t pos[] = {
        {5, 50}, {6, 50}, {8, 30}
    };//+10, +10
    static const char *login_str[] = {
        "Login succeed, you are loged in as %s",
        "Login refused. Would you like to retry? Enter \'N\' for NO, and others for YES.",
        "Login succeed."
    };
    static const char *type_str[] = {
        "Administrator", "Mananger", "Saler"
    };

    int retv = 0;

    ui_draw_login();
    tty_cursor_gotoxy(pos[0].x, pos[0].y);
    ui_getstring(loginmsg.username);
    tty_cursor_gotoxy(pos[1].x, pos[1].y);
    retv += ui_login_getpasswd(loginmsg.passwd);
    retv += srv_user_check(&loginmsg);

    if (retv == RET_SUCCEED) {
        srv_user_login(loginmsg);
        ui_bottom(login_str[2]);
        ui_draw(pos[2].x, pos[2].y, "Login succeed! You has the power of %s.", type_str[loginmsg.type]);
        sleep(1);
        next_ui = ui_main_menu;
        return RET_SUCCEED;
    } else {
        ui_draw(pos[2].x, pos[2].y, login_str[1]);
        int yorn = ui_request(login_str[1]);

        if (yorn == 'N' || yorn == 'n') {
            next_ui = ui_exit;
            return RET_SUCCEED;
        } else {
            next_ui = ui_login;
            return RET_SUCCEED;
        }
    }
    return RET_SUCCEED;
}

int ui_draw_main_menu(int select) {
    ui_window("Theater Ticket Manage System-Main Menu");
    static const ui_pos_t selections[] = {
        {5, 40}, {7, 40}, {9, 40}, {11, 40},
        {13, 40}, {15, 40}, {17, 40}
    };
    static const char *slt_string[] = {
        "1. Studio Management", "2. Play Management",
        "3. Ticket Sale/Return", "4. Queries & Statistics",
        "5. Account Management", "6. Exit"
    };

    for (int i = 0; i < 6; i++) {
        if (select == i) {
            ui_draw_item(selections[i].x, selections[i].y - 1, ">%s", slt_string[i]);
        } else {
            ui_draw(selections[i].x, selections[i].y, slt_string[i]);
        }
    }
    return RET_SUCCEED;
}

int ui_main_menu() {
    static int select = 0;
    int ret;
    do {
        ui_draw_main_menu(select);
        ret = ui_request("Please Select(1 ~ 6):");
        if (ret >= '1' && ret <= '6') {
            select = ret - '1';
        } else if (ret == 'w') {
            select = (select == 0) ? 5 : select - 1;
        } else if (ret == 's') {
            select = (select == 5) ? 0 : select + 1;
        } else if (ret == 10) {
            switch (select) {
            case 0:
                if (this_user.type == 0)
                    next_ui = ui_studio;
                else
                    ui_powerfailed(ui_main_menu);
                return RET_SUCCEED;
            case 1:
                if (this_user.type == 1)
                    next_ui = ui_play;
                else
                    ui_powerfailed(ui_main_menu);
                return RET_SUCCEED;
            case 2:
                if (this_user.type >= 1)
                    next_ui = ui_sale;
                else
                    ui_powerfailed(ui_main_menu);
                return RET_SUCCEED;
            case 3:
                if (this_user.type >= 1)
                    next_ui = ui_query;
                else
                    ui_powerfailed(ui_main_menu);
                return RET_SUCCEED;
            case 4:
                next_ui = (this_user.type >= 1) ? ui_account_lowpower : ui_account;
                return RET_SUCCEED;
            case 5:
                next_ui = ui_exit;
                return RET_SUCCEED;
            }
        }
    } while (1);
    return RET_SUCCEED;
}

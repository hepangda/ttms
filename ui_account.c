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
#include"include/ui.h"
#include"include/frame.h"
#include"include/global.h"
#include"include/tty.h"
#include"include/service.h"
#include<unistd.h>
#include<sys/ioctl.h>
#include<string.h>

extern int (*next_ui)();

int ui_draw_account_changepwd() {
    char input[200], buf[200], buf2[200];
    ui_clearlines(23, 28);

    link_t ret = srv_find_user_name(this_user.username);
    if (ret == NULL) {
        ui_draw_highlight(23, 8, "Unexcepted Error");
        return -1;
    }

    user_t *this = (user_t *)ret->data;

    ui_draw(23, 8, "New Password: ");
    ui_scanf(input, "%s", buf);
    ui_draw(24, 8, "Input New Password Again: ");
    ui_scanf(input, "%s", buf2);

    ui_clearlines(23, 28);
    if (strcmp(buf, buf2) != 0) {
        ui_draw_highlight(23, 8, "Update Failed, input is not the same.");
        return -1;
    } else {
        strcpy(this->passwd, buf2);
        ui_draw_highlight(23, 8, "Update Succeed!");
    }

    return RET_SUCCEED;
}

int ui_account_changepwd() {
    ui_draw_account_changepwd();
    sleep(2);
    next_ui = ui_account_lowpower;
}

int ui_draw_account_lowpower(int select) {
    ui_window("TTMS Account Management");
    static const char *SEPLINE = "================================================================================================";
    static const ui_pos_t selections[] = { {23, 8}, {24, 8} };
    static const char *slt_string[] = {
        "1. Change My Password", "2. Return to Main Menu"
    };

    ui_draw(3, 6, SEPLINE);
    ui_draw(21, 6, SEPLINE);
    ui_draw(10, 20, "You are not administrator, you can only change your password.");
    ui_clearlines(23, 28);
    for (int i = 0; i < 2; i++) {
        if (select == i) {
            ui_draw_item(selections[i].x, selections[i].y - 1, ">%s", slt_string[i]);
        } else {
            ui_draw(selections[i].x, selections[i].y, slt_string[i]);
        }
    }
}

int ui_account_lowpower() {
    static int select = 0;
    int err_draw = 0;
    int ret;
    do {
        ui_draw_account_lowpower(select);
        if (err_draw) {
            err_draw = 0;
            ui_bottom("Your choice is out of range.");
        }
        ret = ui_request("You aren\'t administrator, you can only change your password.");
        if (ret >= '1' && ret <= '2') {
            select = ret - '1';
        } else if (ret == 'w' || ret == 's') {
            select = (select == 0) ? 1 : 0;
        } else if (ret == 10) {
            next_ui = (select) ? ui_main_menu : ui_account_changepwd;
            return RET_SUCCEED;
        } else {
            err_draw = 1;
        }
    } while (1);
    return RET_SUCCEED;
}

int ui_draw_account(int select, int pages) {
    ui_window("TTMS Account Management");
    static const char *SEPLINE = "================================================================================================";
    static const ui_pos_t selections[] = {
        {23, 8}, {23, 38}, {23, 68}, {24, 8},
        {24, 38}, {24, 68}, {25, 8}
    };
    static const char *slt_string[] = {
        "1. Prev Page", "2. Next Page",
        "3. Add User", "4. Delete User",
        "5. Update User", "6. Find User",
        "7. Return to Main Menu"
    };
    static const char *chart_title_format = "%-30s %-20s";
    static const char *chart_list_format = "%-30s %-6d";

    ui_draw(3, 6, SEPLINE);
    ui_draw(5, 6, SEPLINE);
    ui_draw(21, 6, SEPLINE);
    ui_draw(4, 8, chart_title_format, "User Name", "User Type");
    ui_draw_highlight(22, 8, "Page: %d", pages);

    link_t first = ui_pager(g_user, pages);
    for (int i = 0; i < UI_ITEM_PERPAGE && first != NULL; i++, first = first->next) {
        user_t *this = (user_t *)first->data;
        ui_draw(6 + i, 8, chart_list_format, this->username, this->type)
    }

    for (int i = 0; i < 7; i++) {
        if (select == i) {
            ui_draw_item(selections[i].x, selections[i].y - 1, ">%s", slt_string[i]);
        } else {
            ui_draw(selections[i].x, selections[i].y, slt_string[i]);
        }
    }
}

int ui_account() {
    static int select = 0;
    static int pages = 1;
    int max_page = ui_pager_maxpage(g_user);
    int err_draw = 0;
    int ret;
    do {
        ui_draw_account(select, pages);
        if (err_draw) {
            err_draw = 0;
            ui_bottom("Your choice is out of range.");
        }
        ret = ui_request("Please Select(0 ~ 6):");
        if (ret >= '1' && ret <= '7') {
            select = ret - '1';
        } else if (ret == 'w') {
            if (select == 0) {
                select = 6;
            } else {
                select--;
            }
        } else if (ret == 's') {
            if (select == 6) {
                select = 0;
            } else {
                select++;
            }
        } else if (ret == 10) {
            switch (select) {
            case 0:
                pages = (pages > 1) ? pages - 1 : pages;
                break;
            case 1:
                pages = (pages <= max_page) ? pages + 1 : pages;
                break;
            case 2:
                next_ui = ui_account_add;
                return 0;
            case 3:
                next_ui = ui_account_delete;
                return 0;
            case 4:
                next_ui = ui_account_update;
                return 0;
            case 5:
                next_ui = ui_account_find;
                return 0;
            case 6:
                next_ui = ui_main_menu;
                return 0;
            }
        } else {
            err_draw = 1;
        }
    } while (1);
}


int ui_draw_account_update() {
    static const ui_pos_t selections[] = {
        {23, 8}, {24, 8}
    };
    static const char *slt_string[] = {
        "New Password", "New Usertype"
    };

    ui_clearlines(23, 28);

    const char *tips = "Enter the username: ";
    tty_cursor_gotoxy(23, 8);
    ui_write(tips);

    char input[200];
    ui_getstring(input);

    link_t ret = srv_find_user_name(input);

    ui_clearlines(23, 28);
    if (ret == NULL) {
        ui_draw_highlight(23, 8, "There\'s no user named %s!", input);
        return -1;
    }

    user_t *this = (user_t *)ret->data;

    ui_draw(selections[0].x, selections[0].y, "%s :", slt_string[0], this->passwd);
    ui_scanf(input, "%s", &this->passwd);

    ui_draw(selections[1].x, selections[1].y,
            "%s[0 for Admin, 1 for Manager, 2 for Saler][Origin %d]: ", slt_string[1], this->type);
    ui_scanf(input, "%d", &this->type);

    ui_clearlines(23, 28);

    if (this->type < 0 || this->type > 2) {
        ui_draw_highlight(23, 8, "User type error! Update error");
        return -1;
    }
    ui_draw_highlight(23, 8, "Update Succeed!");

    return RET_SUCCEED;
}

int ui_account_update() {;
    ui_draw_account_update();
    sleep(2);
    next_ui = ui_account;
}

int ui_draw_account_delete() {
    ui_clearlines(23, 28);
    ui_draw(23, 8, "Enter the username: ");

    char input[200];
    ui_getstring(input);
    link_t ret = srv_find_user_name(input);

    ui_clearlines(23, 28);

    tty_highlight;
    tty_cursor_gotoxy(23, 8);

    if (ret == NULL) {
        ui_draw_highlight(23, 8, "There\'s no user named %s. Delete Failed.", input);
        return -1;
    } else {
        ll_deletea(g_user, input, srv_user_equname);
        ui_draw_highlight(23, 8, "Delete Succeed!");
        return 0;
    }
}

int ui_account_delete() {;
    ui_draw_account_delete();
    sleep(2);
    next_ui = ui_account;
    return RET_SUCCEED;
}


int ui_draw_account_add() {
    static const ui_pos_t selections[] = {
        {23, 8}, {24, 8}, {25, 8}
    };
    static const char *slt_string[] = {
        "Username: ", "Password: ",
        "User Type[0 for Admin, 1 for Manager, 2 for Saler]: "
    };

    ui_clearlines(23, 28);

    char input[200];
    user_t newuser;

    ui_draw(selections[0].x, selections[0].y, slt_string[0]);
    ui_scanf(input, "%s", newuser.username);

    ui_draw(selections[1].x, selections[1].y, slt_string[1]);
    ui_scanf(input, "%s", &newuser.passwd);

    ui_draw(selections[2].x, selections[2].y, slt_string[2]);
    ui_scanf(input, "%d", &newuser.type);

    ui_clearlines(23, 28);

    if (newuser.type < 0 || newuser.type > 2) {
        ui_draw_highlight(23, 8, "User type error! Add Failed.");
        return -1;
    }

    int ret = srv_user_add(newuser);
    if (ret == 0) {
        ui_draw_highlight(23, 8, "Add Succeed!");
    } else {
        ui_draw_highlight(23, 8, "Add Error!");
    };
    return ret;
}


int ui_account_add() {;
    ui_draw_account_add();
    sleep(2);
    next_ui = ui_account;
}

int ui_draw_account_find() {
    ui_clearlines(23, 28);

    const char *tips = "Enter the username: ";
    tty_cursor_gotoxy(23, 8);
    ui_write(tips);

    char input[200];
    ui_getstring(input);
    link_t ret = srv_find_user_name(input);

    ui_clearlines(23, 28);

    tty_highlight;
    if (ret == NULL) {
        ui_draw_highlight(23, 8, "There\'s no user named %s, find failed.", input);
        ui_draw(25, 8, "Press any key to continue...");
        int rets = ui_request("Press any key..");
        return -1;
    } else {
        user_t *this = (user_t *)ret->data;
        ui_draw_highlight(23, 8, "Find Result: %s, user type: %d", this->username, this->type);
        ui_draw(25, 8, "Press any key to continue...");
        int rets = ui_request("Press any key..");
        return RET_SUCCEED;
    }
}

int ui_account_find() {;
    ui_draw_account_find();
    next_ui = ui_account;
    return RET_SUCCEED;
}

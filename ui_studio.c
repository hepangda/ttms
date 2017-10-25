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

extern int (*next_ui)();

int ui_draw_studio(int select, int pages) {
    ui_window("TTMS Studio Management");
    static const char *SEPLINE = "================================================================================================";
    static const ui_pos_t selections[] = {
        {23, 8}, {23, 38}, {23, 68}, {24, 8},
        {24, 38}, {24, 68}, {25, 8}
    };
    static const char *slt_string[] = {
        "1. Prev Page", "2. Next Page",
        "3. Add Studio", "4. Delete Studio",
        "5. Update Studio", "6. Seat Management",
        "7. Return to Main Menu"
    };
    static const char *chart_title_format = "|%-6s|%-30s|%-8s|%-8s|%-10s|",
                      *chart_list_format = "|%-6d|%-30s|%-8d|%-8d|%-10d|";

    ui_draw(3, 6, SEPLINE);
    ui_draw(5, 6, SEPLINE);
    ui_draw(21, 6, SEPLINE);
    ui_draw(4, 8, chart_title_format, "ID", "Name", "Rows", "Cols", "Seats");
    ui_draw_highlight(22, 8, "Page: %d", pages);


    link_t first = ui_pager(g_studio, pages);
    for (int i = 0; i < UI_ITEM_PERPAGE && first != NULL; i++, first = first->next) {
        studio_t *this = (studio_t *)first->data;
        ui_draw(6 + i, 8, chart_list_format, this->id, this->name, this->rows, this->cols, this->seats);
    }

    for (int i = 0; i < 7; i++) {
        if (select == i) {
            ui_draw_item(selections[i].x, selections[i].y - 1, ">%s", slt_string[i]);
        } else {
            ui_draw(selections[i].x, selections[i].y, slt_string[i]);
        }
    }
    return RET_SUCCEED;
}

int ui_studio() {
    static int select = 0;
    static int pages = 1;
    int max_page = ui_pager_maxpage(g_studio);
    int err_draw = 0;
    int ret;
    do {
        ui_draw_studio(select, pages);
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
                next_ui = ui_studio_add;
                return 0;
            case 3:
                next_ui = ui_studio_delete;
                return 0;
            case 4:
                next_ui = ui_studio_update;
                return 0;
            case 5:
                next_ui = ui_seat_ask;
                return 0;
            case 6:
                next_ui = ui_main_menu;
                return 0;
            }
        } else {
        }
    } while (1);
    return RET_SUCCEED;
}

int ui_draw_studio_update() {
    static const ui_pos_t selections[] = {
        {23, 8}, {24, 8}, {25, 8}
    };
    static const char *slt_string[] = {
        "Studio Name", "Row", "Column"
    };

    ui_clearlines(23, 28);
    ui_draw(23, 8, "Enter the ID of Studio: ");
    char input[200];
    int id = -1;
    ui_scanf(input, "%d", &id);

    link_t ret = srv_find_studio_id(id);

    if (ret == NULL) {
        ui_clearlines(23, 28);
        ui_draw_highlight(23, 8, "Error ID! Update failed.");
        return -1;
    }

    studio_t *this = (studio_t *)ret->data;

    ui_draw(selections[0].x, selections[0].y, "%s[Origin \"%s\"]: ", slt_string[0], this->name);
    ui_scanf(input, "%s", this->name);

    ui_draw(selections[1].x, selections[1].y, "%s[Origin %d]: ", slt_string[1], this->rows);
    ui_scanf(input, "%d", &this->rows);

    ui_draw(selections[2].x, selections[2].y, "%s[Origin %d]: ", slt_string[0], this->cols);;
    ui_scanf(input, "%d", &this->cols);

    ui_clearlines(23, 28);
    srv_seat_studiodel(this->id);
    srv_seat_studioadd(this->id, this->rows, this->cols);

    ui_draw_highlight(23, 8, "Update Succeed!");
    return -1;
}

int ui_studio_update() {;
    ui_draw_studio_update();
    sleep(2);
    next_ui = ui_studio;
}

int ui_draw_studio_delete() {
    ui_clearlines(23, 28);

    ui_draw(23, 8, "Enter the ID of Studio: ");
    char input[200];
    int id = -1;
    ui_scanf(input, "%d", &id);

    link_t ret = srv_find_studio_id(id);
    ui_clearlines(23, 28);

    if (ret == NULL) {
        ui_draw_highlight(23, 8, "Error ID! Delete Failed.");
        return -1;
    } else {
        ll_delete(g_studio, id, srv_studio_equid);
        srv_seat_studiodel(id);
        ui_draw_highlight(23, 8, "Delete Succeed!");
        return RET_SUCCEED;
    }
}

int ui_studio_delete() {;
    ui_draw_studio_delete();
    sleep(2);
    next_ui = ui_studio;
}


int ui_draw_studio_add() {
    static const ui_pos_t selections[] = {
        {23, 8}, {24, 8}, {25, 8}
    };
    static const char *slt_string[] = {
        "Studio Name: ", "Row: ", "Column: "
    };
    ui_clearlines(23, 28);
    char input[200];
    studio_t *last = (studio_t *)g_studio.pend->data;

    int id;
    if (last == NULL) {
        id = 0;
    } else {
        id = last->id + 1;
    }

    studio_t newstudio;
    newstudio.id = id;

    ui_draw(selections[0].x, selections[0].y, slt_string[0]);
    ui_scanf(input, "%s", newstudio.name);

    ui_draw(selections[1].x, selections[1].y, slt_string[1]);
    ui_scanf(input, "%d", &newstudio.rows);

    ui_draw(selections[2].x, selections[2].y, slt_string[2]);
    ui_scanf(input, "%d", &newstudio.cols);

    newstudio.seats = newstudio.rows * newstudio.cols;

    int ret = srv_studio_add(newstudio);

    ui_clearlines(23, 28);
    if (ret == 0) {
        ui_draw_highlight(23, 8, "Add Succeed!");
    } else {
        ui_draw_highlight(23, 8, "Add Failed!");
    }
    return ret;
}

int ui_studio_add() {;
    ui_draw_studio_add();
    sleep(2);
    next_ui = ui_studio;
}

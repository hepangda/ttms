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
#include"include/ttms_ui_func.h"
#include"include/ttms_global.h"
#include"include/ttms_tty.h"
#include"include/ttms_srv.h"
#include<unistd.h>
#include<sys/ioctl.h>

extern int (*next_ui)();

int ui_draw_play(int select, int pages) {
    ui_window("TTMS Play Management");
    static const char *SEPLINE = "=============================================================================================================";
    static const ui_pos_t selections[] = {
        {23, 8}, {23, 38}, {23, 68}, {24, 8},
        {24, 38}, {24, 68}, {25, 8}
    };
    static const char *slt_string[] = {
        "1. Prev Page", "2. Next Page",
        "3. Add Play", "4. Delete Play",
        "5. Update Play", "6. Schedule Management",
        "7. Return to Main Menu"
    };
    static const char *type_str[] = {
        "FILE", "OPERA", "CONC"
    };
    static const char *rating_str[] = {
        "ALL", "14+", "18+"
    };
    static const char *chart_title_format = "|%-6s|%-30s|%-5s|%-5s|%-8s|%-10s|%-10s|%-10s|%-6s|";
    static const char *chart_list_format = "|%-6d|%-30s|%-5s|%-5s|%-8s|%-10d|%4d/%02d/%02d|%4d/%02d/%02d|%-6d|";

    ui_draw(3, 2, SEPLINE);
    ui_draw(5, 2, SEPLINE);
    ui_draw(21, 2, SEPLINE);
    ui_draw(4, 4, chart_title_format, "ID", "Name", "Type", "Area", "Rating",
            "Duration", "From", "To", "Price");
    ui_draw_highlight(22, 4, "Page: %d", pages);



    dstruct_linklist_link first = ui_pager(g_play, pages);
    for (int i = 0; i < UI_ITEM_PERPAGE && first != NULL; i++, first = first->next) {
        play_t *this = (play_t *)first->data;
        ui_draw(6 + i, 4, chart_list_format, this->id, this->name, type_str[this->type],
                this->area, rating_str[this->rating], this->duration, this->start_date.year,
                this->start_date.month, this->start_date.day,
                this->end_date.year, this->end_date.month,
                this->end_date.day, this->price);
    }

    for (int i = 0; i < 7; i++) {
        if (select == i) {
            ui_draw_item(selections[i].x, selections[i].y - 1, ">%s", slt_string[i]);
        } else {
            ui_draw(selections[i].x, selections[i].y, slt_string[i]);
        }
    }
}

int ui_play() {
    static int select = 0;
    static int pages = 1;
    int max_page = ui_pager_maxpage(g_play);
    int err_draw = 0;
    int ret;
    do {
        ui_draw_play(select, pages);
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
                next_ui = ui_play_add;
                return 0;
            case 3:
                next_ui = ui_play_delete;
                return 0;
            case 4:
                next_ui = ui_play_update;
                return 0;
            case 5:
                next_ui = ui_schedule;
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


int ui_draw_play_update() {
    char input[200];
    int id = -1;
    static const ui_pos_t selections[] = {
        {23, 8}, {24, 8}, {25, 8},
        {23, 8}, {24, 8}, {25, 8},
        {26, 8}, {27, 8}
    };
    static const char *slt_string[] = {
        "Play Name", "Play Type", "Area",
        "Rating", "Duration", "Start Date",
        "End Date", "Price"
    };

    ui_clearlines(23, 28);
    ui_draw(23, 8, "Enter the ID of Play: ");
    ui_scanf(input, "%d", &id);

    dstruct_linklist_link ret = srv_find_play_id(id);

    if (ret == NULL) {
        ui_draw_highlight(23, 8, "Error ID! Update Failed.");
        return -1;
    }

    play_t *this = (play_t *)ret->data;

    ui_draw(selections[0].x, selections[0].y, "%s[Origin \"%s\"]: ", slt_string[0], this->name);
    ui_scanf(input, "%s", this->name);

    ui_draw(selections[1].x, selections[1].y,
            "%s[0 for File, 1 for Opera, 2 for concept][Origin %d]:", slt_string[1], this->type);
    ui_scanf(input, "%d", &this->type);

    ui_draw(selections[2].x, selections[2].y, "%s[Origin %s]: ", slt_string[2], this->area);
    ui_scanf(input, "%s", this->area);

    ui_clearlines(23, 28);

    ui_draw(selections[3].x, selections[3].y,
            "%s[0 for Child, 1 for Teenager, 2 for Adult][Origin %d]: ", slt_string[3], this->rating);
    ui_scanf(input, "%d", &this->rating);

    ui_draw(selections[4].x, selections[4].y, "%s[Origin %d]: ", slt_string[4], this->duration);
    ui_scanf(input, "%d", &this->duration);

    ui_draw(selections[5].x, selections[5].y, "%s[Format:YYYY/MM/DD][Origin %d/%02d/%02d]: ",
            slt_string[5], this->start_date.year, this->start_date.month, this->start_date.day);
    ui_scanf(input, "%d/%d/%d", &this->start_date.year,
           &this->start_date.month, &this->start_date.day);

    ui_draw(selections[6].x, selections[6].y, "%s[Format:YYYY/MM/DD][Origin %d/%02d/%02d]: ",
            slt_string[6], this->end_date.year, this->end_date.month, this->end_date.day);
    ui_scanf(input, "%d/%d/%d", &this->end_date.year,
           &this->end_date.month, &this->end_date.day);

    ui_draw(selections[7].x, selections[7].y, "%s[Origin %d]: ", slt_string[7], this->price);
    ui_scanf(input, "%d", &this->price);

    ui_clearlines(23, 28);
    ui_draw_highlight(23, 8, "Update Succeed!");
    return -1;
}

int ui_play_update() {;
    ui_draw_play_update();
    sleep(2);
    next_ui = ui_play;
}

int ui_draw_play_delete() {
    char input[200];
    int id = -1;
    ui_clearlines(23, 28);
    ui_draw(23, 8, "Enter the ID of Play: ");
    ui_scanf(input, "%d", &id);

    dstruct_linklist_link ret = srv_find_play_id(id);

    ui_clearlines(23, 28);

    if (ret == NULL) {
        ui_draw_highlight(23, 8, "Error ID! Delete Failed.");
        return -1;
    } else {
        g_studio.delete(g_play.this, (void *)&id, srv_play_equid);
        ui_draw_highlight(23, 8, "Delete Succeed.");
    }
    return RET_SUCCEED;
}

int ui_play_delete() {;
    ui_draw_play_delete();
    sleep(2);
    next_ui = ui_play;
}

int ui_draw_play_add() {
    char input[200];
    static const ui_pos_t selections[] = {
        {23, 8}, {24, 8}, {25, 8},
        {23, 8}, {24, 8}, {25, 8},
        {26, 8}, {27, 8}
    };
    static const char *slt_string[] = {
        "Play Name: ", "Play Type[0 for File, 1 for Opera, 2 for concept]: ", "Area: ",
        "Rating[0 for Child, 1 for Teenager, 2 for Adult]: ", "Duration: ",
        "Start Date[Format:YYYY/MM/DD]: ", "End Date[Format:YYYY/MM/DD]: ", "Price: "
    };
    ui_clearlines(23, 28);

    play_t *idfinder = (play_t *)g_play.dll_pend->data;
    play_t this;
    if (idfinder == NULL) {
        this.id = 1;
    } else {
        this.id = idfinder->id + 1;
    }

    ui_draw(selections[0].x, selections[0].y, slt_string[0]);
    ui_scanf(input, "%s", this.name);

    ui_draw(selections[1].x, selections[1].y, slt_string[1]);
    ui_scanf(input, "%d", &this.type);

    ui_draw(selections[2].x, selections[2].y, slt_string[2]);
    ui_scanf(input, "%s", this.area);

    ui_clearlines(23, 28);

    ui_draw(selections[3].x, selections[3].y, slt_string[3]);
    ui_scanf(input, "%d", &this.rating);

    ui_draw(selections[4].x, selections[4].y, slt_string[4]);
    ui_scanf(input, "%d", &this.duration);

    ui_draw(selections[5].x, selections[5].y, slt_string[5]);
    ui_scanf(input, "%d/%d/%d", &this.start_date.year, &this.start_date.month, &this.start_date.day);

    ui_draw(selections[6].x, selections[6].y, slt_string[6]);
    ui_scanf(input, "%d/%d/%d", &this.end_date.year, &this.end_date.month, &this.end_date.day);

    ui_draw(selections[7].x, selections[7].y, slt_string[7]);
    ui_scanf(input, "%d", &this.price);

    int ret = srv_play_add(this);
    ui_clearlines(23, 28);
    if (ret != RET_SUCCEED) {
        ui_draw_highlight(23, 28, "Add Error!");
        return -1;

    } else {
        ui_draw_highlight(23, 28, "Add Succeed!");
    }
    return RET_SUCCEED;
}

int ui_play_add() {;
    ui_draw_play_add();
    sleep(2);
    next_ui = ui_play;
}

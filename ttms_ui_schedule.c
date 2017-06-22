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

#include<assert.h>
extern int (*next_ui)();

int ui_draw_schedule(int select, int pages) {
    ui_window("TTMS Schedule Management");
    static const char *SEPLINE = "=============================================================================================================";
    static const ui_pos_t selections[] = {
        {23, 8}, {23, 38}, {23, 68},
        {24, 8}, {24, 38}, {24, 68}
    };
    static const char *slt_string[] = {
        "1. Prev Page", "2. Next Page",
        "3. Add Schedule", "4. Delete Schedule",
        "5. Update Schedule", "6. Return to Main Menu"
    };

    static const char *chart_title_format = "|%-6s|%-30s|%-10s|%-10s|%-8s|%-6s|";
    static const char *chart_list_format = "|%-6d|%-30s|%-10d|%4d/%02d/%02d|%02d:%02d:%02d|%-6d|";

    ui_draw(3, 2, SEPLINE);
    ui_draw(5, 2, SEPLINE);
    ui_draw(21, 2, SEPLINE);
    ui_draw(4, 4, chart_title_format, "ID", "Play Name", "Studio ID", "Date", "Times", "Seats")
    ui_draw_highlight(22, 4, "Page: %d", pages);

    dstruct_linklist_link first = ui_pager(g_schedule, pages);
    for (int i = 0; i < UI_ITEM_PERPAGE && first != NULL; i++, first = first->next) {
        schedule_t *this = (schedule_t *)first->data;
        studio_t *this_studio = (studio_t *)(srv_find_studio_id(this->studio_id)->data);
        play_t *this_play = (play_t *)(srv_find_play_id(this->play_id)->data);
        ui_draw(6 + i, 4, chart_list_format, this->id, this_play->name, this->studio_id,
                this->date.year, this->date.month, this->date.day, this->time.hour,
                this->time.min, this->time.sec, this_studio->seats);
    }

    for (int i = 0; i < 6; i++) {
        if (select == i) {
            ui_draw_item(selections[i].x, selections[i].y - 1, ">%s", slt_string[i]);
        } else {
            ui_draw(selections[i].x, selections[i].y, slt_string[i]);
        }
    }
}

int ui_schedule() {
    static int select = 0;
    static int pages = 1;
    int max_page = ui_pager_maxpage(g_schedule);
    int err_draw = 0;
    int ret;
    do {
        ui_draw_schedule(select, pages);
        if (err_draw) {
            err_draw = 0;
            ui_bottom("Your choice is out of range.");
        }
        ret = ui_request("Please Select(0 ~ 6):");
        if (ret >= '1' && ret <= '6') {
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
                next_ui = ui_schedule_add;
                return 0;
            case 3:
                next_ui = ui_schedule_delete;
                return 0;
            case 4:
                next_ui = ui_schedule_update;
                return 0;
            case 5:
                next_ui = ui_play;
                return 0;
            }
        } else {
            err_draw = 1;
        }
    } while (1);
}


int ui_draw_schedule_update() {
    char input[200];
    int id = -1;
    static const ui_pos_t selections[] = {
        {23, 8}, {24, 8}, {25, 8}, {26, 8}, {27, 8}
    };
    static const char *slt_string[] = {
        "Play ID", "Studio ID", "Date", "Time"
    };

    ui_clearlines(23, 28);
    ui_draw(23, 8, "Enter the ID of Schedule: ");
    ui_scanf(input, "%d", &id);

    dstruct_linklist_link ret = srv_find_schedule_id(id);

    ui_clearlines(23, 28);
    if (ret == NULL) {
        ui_draw_highlight(23, 8, "Error ID! Update Failed.");
        return -1;
    }

    schedule_t *this = (schedule_t *)ret->data;
    //int ticket_price = 0;
    ui_draw(selections[0].x, selections[0].y, "%s[Origin %d]: ", slt_string[0], this->play_id);
    ui_scanf(input, "%d", &this->play_id);

    ui_draw(selections[1].x, selections[1].y, "%s[Origin %d]:", slt_string[1], this->studio_id);
    ui_scanf(input, "%d", &this->studio_id);

    ui_draw(selections[2].x, selections[2].y, "%s[Format:YYYY/MM/DD][Origin %d/%02d/%02d]: ",
            slt_string[2], this->date.year, this->date.month, this->date.day);
    ui_scanf(input, "%d/%d/%d", &this->date.year, &this->date.month, &this->date.day);

    ui_draw(selections[3].x, selections[3].y, "%s[Format:HH:MM:SS][Origin %02d:%02d:%02d]: ",
            slt_string[3], this->time.hour, this->time.min, this->time.sec);
    ui_scanf(input, "%d:%d:%d", &this->time.hour, &this->time.min, &this->time.sec);

    ui_clearlines(23, 28);
    ui_draw_highlight(23, 8, "Update Succeed!");
    return RET_SUCCEED;
}

int ui_schedule_update() {;
    ui_draw_schedule_update();
    sleep(2);
    next_ui = ui_schedule;
}

int ui_draw_schedule_delete() {
    char input[200];
    int id = -1;
    ui_clearlines(23, 28);
    ui_draw(23, 8, "Enter the ID of Schedule: ");
    ui_scanf(input, "%d", &id);

    dstruct_linklist_link ret = srv_find_schedule_id(id);

    ui_clearlines(23, 28);
    if (ret == NULL) {
        ui_draw_highlight(23, 8, "Error ID! Delete Failed.");
        return -1;
    } else {
        g_schedule.delete(g_schedule.this, (void *)&id, srv_schedule_equid);
        srv_ticket_scheduledel(id);
        ui_draw_highlight(23, 8, "Delete Succeed.");;
    }
    return RET_SUCCEED;
}

int ui_schedule_delete() {;
    ui_draw_schedule_delete();
    sleep(2);
    next_ui = ui_schedule;
}

int ui_draw_schedule_add() {
    static const ui_pos_t selections[] = {
        {23, 8}, {24, 8}, {25, 8}, {26, 8}, {27, 8}
    };
    static const char *slt_string[] = {
        "Play ID: ", "Studio ID: ", "Date[Format:YYYY/MM/DD]: ", "Time[Format:HH:MM:SS]: "
    };

    ui_clearlines(23, 28);

    schedule_t *idfinder = (schedule_t *)g_schedule.dll_pend->data;
    schedule_t this;
    dstruct_linklist_link verify;
    this.seat_count = 0;

    if (idfinder == NULL) {
        this.id = 1;
    } else {
        this.id = idfinder->id + 1;
    }
    char input[200];
    int ret = 0;
    ui_draw(selections[0].x, selections[0].y, slt_string[0]);
    ui_scanf(input, "%d", &this.play_id);
    verify = srv_find_play_id(this.play_id);
    if (verify == NULL) ret++;

    ui_draw(selections[1].x, selections[1].y, slt_string[1]);
    ui_scanf(input, "%d", &this.studio_id);
    verify = srv_find_studio_id(this.studio_id);
    if (verify == NULL) ret++;

    ui_draw(selections[2].x, selections[2].y, slt_string[2]);
    ui_scanf(input, "%d/%d/%d", &this.date.year, &this.date.month, &this.date.day);

    ui_draw(selections[3].x, selections[3].y, slt_string[3]);
    ui_scanf(input, "%d:%d:%d", &this.time.hour, &this.time.min, &this.time.sec);

    ret = (ret == 0) ? srv_schedule_add(this) : ret;
    ui_clearlines(23, 28);

    if (ret == 0) {
        ui_draw_highlight(23, 8, "Add Succeed!");
    } else {
        ui_draw_highlight(23, 8, "Add Failed!");
    }
    return ret;
}

int ui_schedule_add() {;
    ui_draw_schedule_add();
    sleep(2);
    next_ui = ui_schedule;
}

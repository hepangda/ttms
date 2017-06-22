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
#include<string.h>

#define UI_FLAG_SCHE (0)
#define UI_FLAG_TICK (1)
#define UI_FLAG_NOTSLT (2)

extern int (*next_ui)();
static int studio_id = -1;
datastruct_create_static_linklist(schedules, schedule_t);
datastruct_create_static_linklist(tickets, ticket_t);
static int flag = UI_FLAG_NOTSLT;
static const char *SEPLINE = "================================================================================================";
static char question[200];
static const char *TIPS[] = {
    "Please Input Play Name:",
    "Please Input Schedule ID:"
};

static int ui_draw_sale_flag(int pages) {
    if (flag == UI_FLAG_SCHE) {
        srv_sale_build_schedule(&schedules, question);
        static const char *chart_title_format = "|%-6s|%-30s|%-10s|%-10s|%-8s|%-6s|";
        static const char *chart_list_format = "|%-6d|%-30s|%-10d|%4d/%02d/%02d|%02d:%02d:%02d|%-6d|";

        ui_draw(3, 2, SEPLINE);
        ui_draw(5, 2, SEPLINE);
        ui_draw(4, 4, chart_title_format, "ID", "Play Name", "Studio ID", "Date", "Times", "Seats");
        ui_draw_highlight(22, 4, "Page: %d", pages);


        dstruct_linklist_link first = ui_pager(schedules, pages);
        tty_cursor_gotoxy(8, 4);
        for (int i = 0; i < UI_ITEM_PERPAGE && first != NULL; i++, first = first->next) {
            schedule_t *this = (schedule_t *)first->data;
            studio_t *this_studio = (studio_t *)(srv_find_studio_id(this->studio_id)->data);
            play_t *this_play = (play_t *)(srv_find_play_id(this->play_id)->data);
            ui_draw(6 + i, 4, chart_list_format, this->id, this_play->name, this->studio_id,
                    this->date.year, this->date.month, this->date.day, this->time.hour,
                    this->time.min, this->time.sec, this_studio->seats);
        }
    } else if (flag == UI_FLAG_TICK) {
        srv_sale_build_ticket(&tickets, question);
        static const char *chart_title_format = "|%-12s|%-12s|%-6s|%-6s|%-10s|%-15s|";
        static const char *chart_list_format = "|%-12d|%-12d|%-6d|%-6d|%-10s|%-15d|";
        static const char *status_str[] = { "Not Saled", "Saled" };

        ui_draw(3, 2, SEPLINE);
        ui_draw(5, 2, SEPLINE);
        ui_draw(4, 4, chart_title_format, "Ticket No", "Schedule ID", "Row",
                "Col", "Status", "Ticket Price");
        ui_draw_highlight(22, 4, "Page: %d", pages);

        dstruct_linklist_link first = ui_pager(tickets, pages);
        for (int i = 0; i < UI_ITEM_PERPAGE && first != NULL; i++, first = first->next) {
            ticket_t *this = (ticket_t *)first->data;
            ui_draw(6 + i, 4, chart_list_format, this->id, this->schedule_id, this->row,
                    this->col, status_str[this->status], this->price);
        }
    } else {
        ui_draw(3, 2, SEPLINE);
        ui_draw_highlight(10, 40, "WELCOME TO USE SALE SYSTEM.");
    }
}

int ui_draw_sale(int select, int pages) {
    ui_window("TTMS Ticket Sale/Return");

    static const ui_pos_t selections[] = {
        {23, 8}, {23, 38}, {23, 68},
        {24, 8}, {24, 38}, {24, 68},
        {25, 8}
    };
    static const char *slt_string[] = {
        "1. Prev Pages", "2. Next Pages",
        "3. Select Schedule on Play", "4. Show Tickets",
        "5. Sale", "6. Return Tickets", "7. Return to Main Menu"
    };

    ui_draw_sale_flag(pages);
    ui_draw(20, 2, SEPLINE);

    for (int i = 0; i < 7; i++) {
        if (select == i) {
            ui_draw_item(selections[i].x, selections[i].y - 1, ">%s", slt_string[i]);
        } else {
            ui_draw(selections[i].x, selections[i].y, slt_string[i]);
        }
    }
}

int ui_draw_sale_sale() {
    char input[200];
    int id = -1;
    static const ui_pos_t selections[] = {
        {23, 8}, {24, 8}
    };
    static const char *slt_string[] = {
        "Row: ", "Col: "
    };

    ui_clearlines(23, 28);
    ui_draw(23, 8, "Enter the ID of Schedule");
    ui_scanf(input, "%d", &id);

    dstruct_linklist_link ret = srv_find_schedule_id(id);

    ui_clearlines(23, 28);

    if (ret == NULL) {
        ui_draw_highlight(23, 8, "Error ID");
        return -1;
    }

    int thisrow = -1, thiscol = -1;

    ui_draw(selections[0].x, selections[0].y, slt_string[0]);
    ui_scanf(input, "%d", &thisrow);

    ui_draw(selections[1].x, selections[1].y, slt_string[1]);
    ui_scanf(input, "%d", &thiscol);

    ticket_t *this = (ticket_t *)((srv_find_ticket(id, thisrow, thiscol))->data);

    ui_clearlines(23, 28);

    if (this != NULL && this->status != 1) {
        srv_sale_ticket(this);
        ui_draw_highlight(23, 8, "Sale Accepted");
    } else {
        ui_draw_highlight(23, 8, "This ticket has been sold.");
    }

    return RET_SUCCEED;
}

int ui_sale_sale() {
    ui_draw_sale_sale();
    sleep(2);
    next_ui = ui_sale;
}

int ui_sale() {
    static int select = 0;
    static int pages = 0;
    int err_draw = 0;
    int ret;
    do {
        int max_page;
        ui_draw_sale(select, pages);
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
                max_page = (flag == UI_FLAG_SCHE) ? ui_pager_maxpage(schedules) : ui_pager_maxpage(tickets);
                pages = (pages > 1) ? pages - 1 : pages;
                break;
            case 1:
                max_page = (flag == UI_FLAG_SCHE) ? ui_pager_maxpage(schedules) : ui_pager_maxpage(tickets);
                pages = (pages <= max_page) ? pages + 1 : pages;
                break;
            case 2:
                next_ui = ui_sale_schedule;
                return 0;
            case 3:
                next_ui = ui_sale_ticket;
                return 0;
            case 4:
                if (this_user.type == 2)
                    next_ui = ui_sale_sale;
                else
                    ui_powerfailed(ui_sale);
                return 0;
            case 5:
                if (this_user.type == 2)
                    next_ui = ui_sale_ret;
                else
                    ui_powerfailed(ui_sale);
                return 0;
            case 6:
                next_ui = ui_main_menu;
                return 0;
            }
        } else {
        }
    } while (1);
}


static int ui_draw_sale_ask(int tips) {
    ui_clearlines(23, 28);
    ui_draw(23, 8, TIPS[tips]);
	ui_scanf(question, "%s", question);
}

int ui_sale_ask(int which) {;
    ui_draw_sale_ask(which);
    next_ui = ui_sale;
}

int ui_sale_schedule() {
    ui_sale_ask(0);
    srv_sale_build_schedule(&schedules, question);
    flag = UI_FLAG_SCHE;
    next_ui = ui_sale;
}

int ui_sale_ticket() {
    ui_sale_ask(1);
    srv_sale_build_ticket(&tickets, question);
    flag = UI_FLAG_TICK;
    next_ui = ui_sale;
}

int ui_draw_sale_ret() {
    char input[200];
    int id = -1;
    static const ui_pos_t selections[] = {
        {23, 8}, {24, 8}
    };
    static const char *slt_string[] = {
        "Row: ", "Col: "
    };

    ui_clearlines(23, 28);

    ui_draw(23, 8, "Enter the ID of Schedule: ");
    ui_scanf(input, "%d", &id);

    dstruct_linklist_link ret = srv_find_schedule_id(id);

    ui_clearlines(23, 28);

    if (ret == NULL) {
        ui_draw_highlight(23, 8, "Error ID! Return Failed");
        return -1;
    }

    int thisrow = -1, thiscol = -1;

    ui_draw(selections[0].x, selections[0].y, slt_string[0]);
    ui_scanf(input, "%d", &thisrow);

    ui_draw(selections[1].x, selections[1].y, slt_string[1]);
    ui_scanf(input, "%d", &thiscol);

    ticket_t *this = (ticket_t *)((srv_find_ticket(id, thisrow, thiscol))->data);

    ui_clearlines(23, 28);

    if (this != NULL && this->status != 0) {
        srv_return_ticket(this);
        ui_draw_highlight(23, 8, "Return Accepted");
    } else {
        ui_draw_highlight(23, 8, "This ticket has not been sold.");
    }
    return -1;
}

int ui_sale_ret() {
    ui_draw_sale_ret();
    sleep(2);
    next_ui = ui_sale;
}

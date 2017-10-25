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

#include<assert.h>
extern int (*next_ui)();

int ui_draw_query(int select, int pages) {
    ui_window("TTMS Query");
    static const char *SEPLINE = "================================================================================================";

    static const ui_pos_t selections[] = {
        {23, 8}, {23, 38}, {23, 68},
        {24, 8}, {24, 38}, {24, 68},
        {25, 8}
    };
    static const char *slt_string[] = {
        "1. Prev Pages", "2. Next Pages",
        "3. Play Rank Update", "4. Saler Analysis",
        "5. Return to Main Menu"
    };
    static const char *chart_title_format = "|%-6s|%-30s|%-16s|%-8s|";
    static const char *chart_list_format = "|%-6d|%-30s|%-16d|%-8d|";

    ui_draw(3, 2, SEPLINE);
    ui_draw(5, 2, SEPLINE);
    ui_draw(20, 2, SEPLINE);
    ui_draw(4, 4, chart_title_format, "Rank", "Play Name", "Total Tickets", "Sale");
    ui_draw_highlight(22, 4, "Page: %d", pages);


    link_t first = ui_pager(g_sale_analysis, pages);

    for (int i = 0; i < UI_ITEM_PERPAGE && first != NULL; i++, first = first->next) {
        sale_analysis_t *this = (sale_analysis_t *)first->data;
        ui_draw(6 + i, 4, chart_list_format, (pages - 1) * 7 + 1 + i,
                this->play.name, this->totaltickets, this->sales);
    }

    for (int i = 0; i < 5; i++) {
        if (select == i) {
            ui_draw_item(selections[i].x, selections[i].y - 1, ">%s", slt_string[i]);
        } else {
            ui_draw(selections[i].x, selections[i].y, slt_string[i]);
        }
    }
}

int ui_query() {
    static int select = 0;
    static int pages = 1;
    int err_draw = 0;
    int ret;
    int max_page = ui_pager_maxpage(g_sale_analysis);
    srv_build_saleanalysis();
    do {
        ui_draw_query(select, pages);
        if (err_draw) {
            err_draw = 0;
            ui_bottom("Your choice is out of range.");
        }
        ret = ui_request("Please Select(0 ~ 6):");
        if (ret >= '1' && ret <= '5') {
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
                next_ui = ui_query_update;
                return 0;
            case 3:
                next_ui = ui_query_sa;
                return 0;
            case 4:
                next_ui = ui_main_menu;
                return 0;
            }
        } else {
            err_draw = 1;
        }
    } while (1);
}

int ui_query_update() {
    ui_clearlines(23, 28);
    srv_build_saleanalysis();
    ui_draw_highlight(23, 8, "Update Succeed.");
    sleep(2);
    next_ui = ui_query;
    return RET_SUCCEED;
}

int ui_draw_query_sa() {
    char input[200];
    static const ui_pos_t selections[] = {
        {23, 8}, {24, 8}
    };
    static const char *slt_string[] = {
        "Start Date[Format:YYYY/MM/DD]: ", "End Date[Format:YYYY/MM/DD]: "
    };

    ui_clearlines(23, 28);
    ui_draw(23, 8, "Enter the username: ");
    ui_getstring(input);

    link_t ret = srv_find_user_name(input);

    ui_clearlines(23, 28);
    if (ret == NULL) {
        ui_draw_highlight(23, 8, "No user named %s!", input);
        sleep(2);
        return -1;
    }

    user_t *this = (user_t *)ret->data;
    date_t begin, end;

    ui_draw(selections[0].x, selections[0].y, slt_string[0]);
    ui_scanf(input, "%d/%d/%d", &begin.year, &begin.month, &begin.day);

    ui_draw(selections[1].x, selections[1].y, slt_string[1]);
    ui_scanf(input, "%d/%d/%d", &end.year, &end.month, &end.day);

    ui_clearlines(23, 28);
    ui_draw_highlight(23, 8, "%s has sold %d yuan in this date.",
                      this->username, srv_saler_acc(this, begin, end));
    ui_request("Press any key to continue");
    return RET_SUCCEED;
}

int ui_query_sa() {
    ui_draw_query_sa();
    next_ui = ui_query;
    return RET_SUCCEED;
}

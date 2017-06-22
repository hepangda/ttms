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
static int studio_id = -1;
static int CORRECT = 0;

int ui_draw_seat(int select) {
    ui_window("TTMS Seat Management");
    static const char *SEPLINE = "================================================================================================";
    static const ui_pos_t selections[] = {
        {23, 8}, {23, 58}, {24, 8}, {24, 58}
    };
    static const char *slt_string[] = {
        "1. Add Seat", "2. Delete Seat",
        "3. Update Seat", "4. Return to Studio Management"
    };
    studio_t *this = (studio_t *)((srv_find_studio_id(studio_id))->data);
    //构建seat
    int seat[this->rows][this->cols];
    srv_build_seatarray(studio_id, this->rows, this->cols, seat);

    ui_draw(3, 2, SEPLINE);
    ui_draw(21, 2, SEPLINE);


    for (int i = 1; i <= this->rows; i++) {
        ui_draw_highlight(3 + i, 4, "Row %03d:", i);
        for (int j = 1; j <= this->cols; j++) {
            if (seat[i][j] == SEAT_GOOD) {
                ui_write("%c", '#');
            } else if (seat[i][j] == SEAT_BROKEN) {
                ui_write("%c", '@');
            } else {
                ui_write("%c", ' ');
            }
        }

    }

    for (int i = 0; i < 4; i++) {
        if (select == i) {
            ui_draw_item(selections[i].x, selections[i].y - 1, ">%s", slt_string[i]);
        } else {
            ui_draw(selections[i].x, selections[i].y, slt_string[i]);
        }
    }
}

int ui_seat() {
    static int select = 0;
    int err_draw = 0;
    int ret;
    do {
        ui_draw_seat(select);
        if (err_draw) {
            err_draw = 0;
            ui_bottom("Your choice is out of range.");
        }
        ret = ui_request("Please Select(0 ~ 4):");
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
                next_ui = ui_seat_add;
                return 0;
            case 1:
                next_ui = ui_seat_delete;
                return 0;
            case 2:
                next_ui = ui_seat_update;
                return 0;
            case 3:
                next_ui = ui_studio;
                return 0;
            }
        } else {
        }
    } while (1);
}

int ui_draw_seat_ask() {
    int id = -1;
    char input[200];

    ui_clearlines(23, 28);
    ui_draw(23, 8, "Enter the ID of Studio: ");
    ui_getstring(input);
    sscanf(input, "%d", &id);

    dstruct_linklist_link ret = srv_find_studio_id(id);

    ui_clearlines(23, 28);

    if (ret == NULL) {
        CORRECT = 0;
        ui_draw_highlight(23, 8, "Error ID!");
        sleep(2);
        return -1;
    } else {
        studio_id = id;
        CORRECT = 1;
        return RET_SUCCEED;
    }
}

int ui_seat_ask() {;
    if (ui_draw_seat_ask()) {
        next_ui = ui_studio;
    } else {
        next_ui = ui_seat;
    }
    return RET_SUCCEED;
}

int ui_draw_seat_update() {
    ui_clearlines(23, 28);
    studio_t *this = (studio_t *)((srv_find_studio_id(studio_id))->data);

    int r, c, set;
    char input[200];

    ui_draw(23, 8, "Which Row:");
    ui_scanf(input, "%d", &r);

    ui_draw(24, 8, "Which Col:");
    ui_scanf(input, "%d", &c);

    ui_draw(25, 8, "Set to [1 for GOOD, 2 for Broken]:");
    ui_scanf(input, "%d", &set);

    dstruct_linklist_link ret;
    if (set != 1 && set != 2) {
        ret = NULL;
    } else {
        ret = srv_find_seat(studio_id, r, c);
    }

    ui_clearlines(23, 28);

    if (ret == NULL) {
        ui_draw_highlight(23, 8, "Update Error! The seat doesn\'t exist.")
        return -1;
    } else {
        seat_t *retv = (seat_t *)ret->data;
        if (retv->status == 1 && set == 2) {
            this->seats--;
        } else if (retv->status == 2 && set == 1) {
            this->seats++;
        }
        srv_find_seat_rc(studio_id, r, c, set);
        ui_draw_highlight(23, 8, "Update Succeed!");
        return 0;
    }
}

int ui_seat_update() {;
    ui_draw_seat_update();
    sleep(2);
    next_ui = ui_seat;
}

int ui_draw_seat_delete() {
    ui_clearlines(23, 28);
    studio_t *this = (studio_t *)((srv_find_studio_id(studio_id))->data);

    int r, c;
    char input[200];

    ui_draw(23, 8, "Which Row:");
    ui_getstring(input);
    sscanf(input, "%d", &r);

    ui_draw(24, 8, "Which Col:");
    ui_getstring(input);
    sscanf(input, "%d", &c);

    dstruct_linklist_link ret = srv_find_seat_rc(studio_id, r, c, SEAT_NONE);

    ui_clearlines(23, 28);

    if (ret == NULL) {
        ui_draw_highlight(23, 8, "Delete Error!", input);
        return -1;
    } else {
        this->seats--;
        ui_draw_highlight(23, 8, "Delete Succeed!", input);
        return 0;
    }
}

int ui_seat_delete() {;
    ui_draw_seat_delete();
    sleep(2);
    next_ui = ui_seat;
}


int ui_draw_seat_add() {
    static const ui_pos_t selections[] = {
        {23, 8}, {24, 8}
    };
    static const char *slt_string[] = {
        "Row: ", "Col: "
    };

    ui_clearlines(23, 28);

    char input[200];
    seat_t *IDFINDER = (seat_t *)g_seat.dll_pend->data;
    seat_t newseat;
    studio_t *this = (studio_t *)((srv_find_studio_id(studio_id))->data);
    newseat.id = IDFINDER->id + 1;
    newseat.roomid = studio_id;
    newseat.status = SEAT_GOOD;

    ui_draw(selections[0].x, selections[0].y, slt_string[0]);
    ui_scanf(input, "%d", &newseat.row);

    ui_draw(selections[1].x, selections[1].y, slt_string[1]);
    ui_scanf(input, "%d", &newseat.col);

    ui_clearlines(23, 28);

    if (newseat.row <= 0 || newseat.row > this->rows ||
        newseat.col <= 0 || newseat.col > this->cols) {
        ui_draw(23, 8, "Add Error: Row or Col Doesn\'t exist!");
        return -1;
    }

    int ret = srv_seat_add(newseat);
    ui_clearlines(23, 28);

    if (ret == 0) {
        this->seats++;
        ui_draw_highlight(23, 8, "Add Succeed!");
    } else {
        ui_draw_highlight(23, 8, "Add Error!");
    }

    return ret;
}


int ui_seat_add() {;
    ui_draw_seat_add();
    sleep(2);
    next_ui = ui_seat;
}

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
#include"include/io.h"
#include"include/global.h"
#include<stdlib.h>
user_t this_user;

static linklist_t *GLOBAL_LINKLISTS[8] = {
    &g_user, &g_studio, &g_seat, &g_play,
    &g_schedule, &g_ticket, &g_sale, &g_sale_analysis
};

void global_initilize() {
    // system("resize -s 29 119");
    ui_initilize();
    for (int i = 0; i < 8; i++) {
        linklist_init(GLOBAL_LINKLISTS[i]);
    }
    io_read_all();
}

void global_exit() {
    io_write_all();
}

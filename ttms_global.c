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
#include"include/ttms_io.h"
#include"include/ttms_global.h"
#include<stdlib.h>
user_t this_user;

void global_initilize() {
    system("resize -s 29 119");
    ui_initilize();
    g_user.dll_init(g_user.this);
    g_studio.dll_init(g_studio.this);
    g_seat.dll_init(g_seat.this);
    g_play.dll_init(g_play.this);
    g_schedule.dll_init(g_schedule.this);
    g_ticket.dll_init(g_ticket.this);
    g_sale.dll_init(g_sale.this);
    g_sale_analysis.dll_init(g_sale_analysis.this);
    io_read_all();
}

void global_exit() {
    io_write_all();
}

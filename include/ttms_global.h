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
#ifndef _TTMS_GLOBAL_H
#define _TTMS_GLOBAL_H
#include"ttms_datastruct.h"
#include"ttms_stddef.h"

extern struct winsize WINSZ;
extern dstruct_linklist g_user;
extern dstruct_linklist g_studio;
extern dstruct_linklist g_seat;
extern dstruct_linklist g_play;
extern dstruct_linklist g_schedule;
extern dstruct_linklist g_ticket;
extern dstruct_linklist g_sale;
extern dstruct_linklist g_sale_analysis;
extern int WATCHDOG;
extern user_t this_user;

void global_initilize();
void global_exit();

#endif

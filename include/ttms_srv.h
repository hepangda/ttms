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

#ifndef _TTMS_SRV_H
#define _TTMS_SRV_H
#include"ttms_datastruct.h"


#define SRV_RET_LOGINFAILED (1)

int srv_user_login(user_t);
int srv_user_check(user_t *);

int srv_sale_build_schedule(dstruct_linklist *, char *);
int srv_sale_build_ticket(dstruct_linklist *, char *);

dstruct_linklist_link srv_find_studio_id(int id);
dstruct_linklist_link srv_find_play_id(int id);
dstruct_linklist_link srv_find_user_name(char *str);
dstruct_linklist_link srv_find_seat_rc(int studio_id, int r, int c, int set);
dstruct_linklist_link srv_find_schedule_id(int id);
int srv_studio_equid(const void *va, const void *vb);
int srv_user_equname(const void *va, const void *vb);
int srv_play_equid(const void *va, const void *vb);
int srv_studio_add(studio_t which);
int srv_user_add(user_t which);
int srv_seat_add(seat_t which);
int srv_play_add(play_t which);

int srv_build_seatarray(int roomid, int row, int col, int dest[row][col]);
int srv_seat_studioadd(int studio_id, int row, int col);
int srv_seat_studiodel(int studio_id);
int srv_seat_equroomid(const void *va, const void *vb);
int srv_schedule_equid(const void *va, const void *vb);
int srv_schedule_add(schedule_t which);
int srv_ticket_scheduleadd(schedule_t which);
int srv_ticket_scheduleid(const void *va, const void *vb);
int srv_ticket_scheduledel(int schedule_id);
dstruct_linklist_link srv_find_seat(int studio_id, int r, int c);
dstruct_linklist_link srv_find_ticket(int schedule_id, int row, int col);

int srv_sale_ticket(ticket_t *which);
int srv_return_ticket(ticket_t *which);
times_t srv_get_nowtime();
date_t srv_get_nowdate();
int srv_build_saleanalysis();
int srv_indate(date_t a, date_t b, date_t c);
int srv_saler_acc(user_t *this, date_t begin, date_t end);
int srv_sort_saleanalysis();
#endif

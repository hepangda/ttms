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
#include"include/ttms_io.h"
#include"include/ttms_global.h"
#include<stdio.h>

int io_read_all() {
    io_read_users();
    io_read_studio();
    io_read_seat();
    io_read_play();
    io_read_schedule();
    io_read_ticket();
    io_read_sale();
    io_read_sale_analysis();
}

int io_write_all() {
    io_write_users();
    io_write_studio();
    io_write_seat();
    io_write_play();
    io_write_schedule();
    io_write_ticket();
    io_write_sale();
    io_write_sale_analysis();
}

int io_read_users() {
    FILE *fp = fopen(IO_FILE_USER, IO_MODE_READ);

    if (fp == NULL) {
        return -1;
    }

    user_t t;
    io_read_any_struct(fp, t, sizeof(user_t));
    while (!feof(fp)) {
        g_user.append(g_user.this, (void *)&t);
        io_read_any_struct(fp, t, sizeof(user_t));
    }

    fclose(fp);
}

int io_write_users() {
    FILE *fp = fopen(IO_FILE_USER, IO_MODE_WRITE);

    if (fp == NULL) {
        return -1;
    }

    dstruct_linklist_iter(g_user) {
        io_write_any_struct(fp, pi->data, sizeof(user_t));
    }

    fflush(fp);
    fclose(fp);
}


int io_read_studio() {
    FILE *fp = fopen(IO_FILE_STUDIO, IO_MODE_READ);

    if (fp == NULL) {
        return -1;
    }

    studio_t t;
    io_read_any_struct(fp, t, sizeof(studio_t));
    while (!feof(fp)) {
        g_studio.append(g_studio.this, (void *)&t);
        io_read_any_struct(fp, t, sizeof(studio_t));
    }

    fclose(fp);
}

int io_write_studio() {
    FILE *fp = fopen(IO_FILE_STUDIO, IO_MODE_WRITE);

    if (fp == NULL) {
        return -1;
    }

    dstruct_linklist_iter(g_studio) {
        io_write_any_struct(fp, pi->data, sizeof(studio_t));
    }

    fflush(fp);
    fclose(fp);
}

int io_read_seat() {
    FILE *fp = fopen(IO_FILE_SEAT, IO_MODE_READ);

    if (fp == NULL) {
        return -1;
    }

    seat_t t;
    io_read_any_struct(fp, t, sizeof(seat_t));
    while (!feof(fp)) {
        g_seat.append(g_seat.this, (void *)&t);
        io_read_any_struct(fp, t, sizeof(seat_t));
    }

    fclose(fp);
}

int io_write_seat() {
    FILE *fp = fopen(IO_FILE_SEAT, IO_MODE_WRITE);

    if (fp == NULL) {
        return -1;
    }

    dstruct_linklist_iter(g_seat) {
        io_write_any_struct(fp, pi->data, sizeof(seat_t));
    }

    fflush(fp);
    fclose(fp);
}

int io_read_play() {
    FILE *fp = fopen(IO_FILE_PLAY, IO_MODE_READ);

    if (fp == NULL) {
        return -1;
    }

    play_t t;
    io_read_any_struct(fp, t, sizeof(play_t));
    while (!feof(fp)) {
        g_play.append(g_play.this, (void *)&t);
        io_read_any_struct(fp, t, sizeof(play_t));
    }

    fclose(fp);
}

int io_write_play() {
    FILE *fp = fopen(IO_FILE_PLAY, IO_MODE_WRITE);

    if (fp == NULL) {
        return -1;
    }

    dstruct_linklist_iter(g_play) {
        io_write_any_struct(fp, pi->data, sizeof(play_t));
    }

    fflush(fp);
    fclose(fp);
}

int io_read_schedule() {
    FILE *fp = fopen(IO_FILE_SCHEDULE, IO_MODE_READ);

    if (fp == NULL) {
        return -1;
    }

    schedule_t t;
    io_read_any_struct(fp, t, sizeof(schedule_t));
    while (!feof(fp)) {
        g_schedule.append(g_schedule.this, (void *)&t);
        io_read_any_struct(fp, t, sizeof(schedule_t));
    }

    fclose(fp);
}

int io_write_schedule() {
    FILE *fp = fopen(IO_FILE_SCHEDULE, IO_MODE_WRITE);

    if (fp == NULL) {
        return -1;
    }

    dstruct_linklist_iter(g_schedule) {
        io_write_any_struct(fp, pi->data, sizeof(schedule_t));
    }

    fflush(fp);
    fclose(fp);
}

int io_read_ticket() {
    FILE *fp = fopen(IO_FILE_TICKET, IO_MODE_READ);

    if (fp == NULL) {
        return -1;
    }

    ticket_t t;
    io_read_any_struct(fp, t, sizeof(ticket_t));
    while (!feof(fp)) {
        g_ticket.append(g_ticket.this, (void *)&t);
        io_read_any_struct(fp, t, sizeof(ticket_t));
    }

    fclose(fp);
}

int io_write_ticket() {
    FILE *fp = fopen(IO_FILE_TICKET, IO_MODE_WRITE);

    if (fp == NULL) {
        return -1;
    }

    dstruct_linklist_iter(g_ticket) {
        io_write_any_struct(fp, pi->data, sizeof(ticket_t));
    }

    fflush(fp);
    fclose(fp);
}

int io_read_sale() {
    FILE *fp = fopen(IO_FILE_SALE, IO_MODE_READ);

    if (fp == NULL) {
        return -1;
    }

    sale_t t;
    io_read_any_struct(fp, t, sizeof(sale_t));
    while (!feof(fp)) {
        g_sale.append(g_sale.this, (void *)&t);
        io_read_any_struct(fp, t, sizeof(sale_t));
    }

    fclose(fp);
}

int io_write_sale() {
    FILE *fp = fopen(IO_FILE_SALE, IO_MODE_WRITE);

    if (fp == NULL) {
        return -1;
    }

    dstruct_linklist_iter(g_sale) {
        io_write_any_struct(fp, pi->data, sizeof(sale_t));
    }

    fflush(fp);
    fclose(fp);
}

int io_read_sale_analysis() {
    FILE *fp = fopen(IO_FILE_SALE_ANALYSIS, IO_MODE_READ);

    if (fp == NULL) {
        return -1;
    }

    sale_analysis_t t;
    io_read_any_struct(fp, t, sizeof(sale_analysis_t));
    while (!feof(fp)) {
        g_sale_analysis.append(g_sale_analysis.this, (void *)&t);
        io_read_any_struct(fp, t, sizeof(sale_analysis_t));
    }

    fclose(fp);
}

int io_write_sale_analysis() {
    FILE *fp = fopen(IO_FILE_SALE_ANALYSIS, IO_MODE_WRITE);

    if (fp == NULL) {
        return -1;
    }

    dstruct_linklist_iter(g_sale_analysis) {
        io_write_any_struct(fp, pi->data, sizeof(sale_analysis_t));
    }

    fflush(fp);
    fclose(fp);
}

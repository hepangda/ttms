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
#include"include/io.h"
#include"include/global.h"
#include<stdio.h>

#define NUMBER_OF_ALL (sizeof(IO_SIZEOF) / sizeof(int))

static const char *IO_MODE_READ = "rb+";
static const char *IO_MODE_WRITE = "wb+";
static const char *IO_FILENAMES[] = {
    "data/Users.dat", "data/Studios.dat", "data/Seats.dat", "data/Plays.dat",
    "data/Schedule.dat", "data/Tickets.dat", "data/Sales.dat", "data/Sale_Analysis.dat"
};

static const int IO_SIZEOF[] = {
    sizeof(user_t), sizeof(studio_t), sizeof(seat_t), sizeof(play_t),
    sizeof(schedule_t), sizeof(ticket_t), sizeof(sale_t), sizeof(sale_analysis_t)
};

static linklist_t *IO_STORE[] = {
    &g_user, &g_studio, &g_seat, &g_play,
    &g_schedule, &g_ticket, &g_sale, &g_sale_analysis
};

int io_read_all() {
    for (int i = 0; i < NUMBER_OF_ALL; i++) {
        io_read(IO_FILENAMES[i], IO_SIZEOF[i], IO_STORE[i]);
    }
}

int io_write_all() {
    for (int i = 0; i < NUMBER_OF_ALL; i++) {
        io_write(IO_FILENAMES[i], IO_SIZEOF[i], IO_STORE[i]);
    }
}

int io_read(const char *filename, const int dsize, linklist_t *store) {
    FILE *fp = fopen(filename, IO_MODE_READ);

    if (fp == NULL) {
        return -1;
    }

    char t[dsize];
    fread((void *)t, dsize, 1, fp);
    while (!feof(fp)) {
        linklist_append(store, (void *)t);
        fread((void *)t, dsize, 1, fp);
    }

    fclose(fp);
    return 0;
}

int io_write(const char *filename, const int dsize, linklist_t *ll) {
    FILE *fp = fopen(filename, IO_MODE_WRITE);

    if (fp == NULL) {
        return -1;
    }

    LINKLIST_ITER((*ll)) {
        fwrite((void *)pi->data, dsize, 1, fp); 
        fflush(fp);
    }

    fflush(fp);
    fclose(fp);
    return 0;
}

#undef NUMBER_OF_ALL
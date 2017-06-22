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
#ifndef _TTMS_IO_H
#define _TTMS_IO_H

#define io_write_any_struct(fp, stup, SIZE) fwrite((void *)stup, SIZE, 1, fp); fflush(fp);
#define io_read_any_struct(fp, store, SIZE) fread((void *)&store, SIZE, 1, fp)

#define IO_MODE_READ ("rb+")
#define IO_MODE_WRITE ("wb+")

#define IO_FILE_USER ("data/Users.dat")
#define IO_FILE_STUDIO ("data/Studios.dat")
#define IO_FILE_SEAT ("data/Seats.dat")
#define IO_FILE_PLAY ("data/Plays.dat")
#define IO_FILE_SCHEDULE ("data/Schedule.dat")
#define IO_FILE_TICKET ("data/Tickets.dat")
#define IO_FILE_SALE ("data/Sales.dat")
#define IO_FILE_SALE_ANALYSIS ("data/Sale_Analysis.dat")

int io_write_all();
int io_read_all();

int io_write_users();
int io_write_studio();
int io_write_seat();
int io_write_play();
int io_write_schedule();
int io_write_ticket();
int io_write_sale();
int io_write_sale_analysis();

int io_read_users();
int io_read_studio();
int io_read_seat();
int io_read_play();
int io_read_schedule();
int io_read_ticket();
int io_read_sale();
int io_read_sale_analysis();

#endif

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
#ifndef _TTMS_HEADER_IO
#define _TTMS_HEADER_IO

#include"linklist.h"

int io_read(const char *filename, const int dsize, linklist_t *store);
int io_write(const char *filename, const int dsize, linklist_t *ll);
int io_write_all();
int io_read_all();

#endif  //_TTMS_HEADER_IO

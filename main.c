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
#include"include/global.h"
#include"include/ui.h"
#include"include/io.h"

int WATCHDOG = 1;
extern int (*next_ui)();

int main(int argc, char *argv[]) {
    global_initilize();
    next_ui = ui_login;
    // strcpy(this_user.username, "admin");
    // strcpy(this_user.passwd, "admin");
    // this_user.type=USER_TYPE_ADMIN;
    while (WATCHDOG) {
        next_ui();
    }
    global_exit();
    return 0;
}

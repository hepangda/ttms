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
#include"include/ttms_stddef.h"
#include"include/ttms_tty.h"
#include<stddef.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>


int tty_disp(int option) {
    int err, fd = STDIN_FILENO;
    struct termios term;

    if (tcgetattr(fd, &term) == -1) {
        return 1;
    }

    if (option) {
        term.c_lflag |= ECHO;
    } else {
        term.c_lflag &= ~ECHO;
    }

    err = tcsetattr(fd, TCSAFLUSH, &term);

    if (err == -1 && err == EINTR) {
        return 1;
    }
    return 0;
}

int tty_cbreak() {
    int err, fd = STDIN_FILENO;
    struct termios buf;

    if (ttystate != RESET) {
        errno = EINVAL;
        return -1;
    }

    if (tcgetattr(fd, &buf) < 0)
        return -1;
    save_termios = buf;

    buf.c_lflag &= ~(ECHO | ICANON);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
        return -1;

    if (tcgetattr(fd, &buf) < 0) {
        err = errno;
        tcsetattr(fd, TCSAFLUSH, &save_termios);
        errno = err;
        return -1;
    }

    if ((buf.c_lflag & (ECHO | ICANON)) || buf.c_cc[VMIN] != 1 ||
        buf.c_cc[VTIME] != 0) {
        tcsetattr(fd, TCSAFLUSH, &save_termios);
        errno = EINVAL;
        return -1;
    }

    ttystate = CBREAK;
    ttysavefd = fd;
    return 0;
}

int tty_raw() {
    int err, fd = STDIN_FILENO;
    struct termios buf;

    if (ttystate != RESET) {
        errno = EINVAL;
        return -1;
    }
    if (tcgetattr(fd, &buf) < 0)
        return -1;
    save_termios = buf;
    buf.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    buf.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    buf.c_cflag &= ~(CSIZE | PARENB);
    buf.c_cflag |= CS8;
    buf.c_oflag &= ~(OPOST);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSAFLUSH, &buf) < 0)
        return -1;

    if (tcgetattr(fd, &buf) < 0) {
        err = errno;
        tcsetattr(fd, TCSAFLUSH, &save_termios);
        errno = err;
        return -1;
    }
    if ((buf.c_lflag & (ECHO | ICANON | IEXTEN | ISIG)) ||
        (buf.c_iflag & (BRKINT | ICRNL | INPCK | ISTRIP | IXON)) ||
        (buf.c_cflag & (CSIZE | PARENB | CS8)) != CS8 ||
        (buf.c_oflag & OPOST) || buf.c_cc[VMIN] != 1 ||
        buf.c_cc[VTIME] != 0) {
        tcsetattr(fd, TCSAFLUSH, &save_termios);
        errno = EINVAL;
        return -1;
    }

    ttystate = RAW;
    ttysavefd = fd;
    return 0;
}

int tty_reset() {
    int fd = STDIN_FILENO;
    if (ttystate == RESET)
        return 0;
    if (tcsetattr(fd, TCSAFLUSH, &save_termios) < 0)
        return -1;
    ttystate = RESET;
    return 0;
}

void tty_atexit() {
    if (ttysavefd >= 0)
        tty_reset(ttysavefd);
}

struct termios *tty_termios() {
    return &save_termios;
}

struct winsize tty_winsize() {
    int fd = STDIN_FILENO;
    struct winsize ret;
    int succeed = ioctl(fd, TIOCGWINSZ, (char *)&ret);
    if (succeed < 0) {
        return ret;
    }
    return ret;
}

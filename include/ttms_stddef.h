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
#ifndef _TTMS_STDDEF_H
#define _TTMS_STDDEF_H
#include<unistd.h>
#define RET_SUCCEED (0)

#define DEF_USERNAME_LENGTH (50)
#define DEF_PASSWD_LENGTH (50)
#define DEF_STUDIO_NAME_LENGTH (30)

typedef struct tag_time {
    int hour, min, sec;
} times_t;


typedef struct tag_date {
    int year, month, day;
} date_t;

typedef enum tag_user_type {
    USER_TYPE_ADMIN,
    USER_TYPE_MANAGER,
    USER_TYPE_SALER
} user_type_t;

typedef struct tag_user {
    char username[DEF_USERNAME_LENGTH + 1];
    char passwd[DEF_PASSWD_LENGTH + 1];
    user_type_t type;
} user_t;

typedef struct tag_studio {
    int id;
    char name[DEF_STUDIO_NAME_LENGTH];
    int rows;
    int cols;
    int seats;
} studio_t;

typedef enum tag_seat_status {
    SEAT_NONE,
    SEAT_GOOD,
    SEAT_BROKEN
} seat_status_t;

typedef struct tag_seat {
    int id;
    int roomid;
    int row;
    int col;
    seat_status_t status;
} seat_t;

typedef enum {
    PLAY_TYPE_FILE, PLAY_TYPE_OPEAR, PLAY_TYPE_CONCERT
} play_type;

typedef enum {
    PLAY_RATE_CHILD, PLAY_RATE_TEENAGE, PLAY_RATE_ADULT
} play_rating;

typedef struct tag_play {
    int id;
    char name[31];
    play_type type;
    char area[9];
    play_rating rating;
    int duration;
    date_t start_date;
    date_t end_date;
    int price;
} play_t;

typedef struct tag_schedule {
    int id;
    int play_id;
    int studio_id;
    date_t date;
    times_t time;
    int seat_count;
} schedule_t;

typedef enum tag_sale_type {
    SALE_SELL,
    SALE_RETURN
} sale_type_t;

typedef struct tag_ticket {
    int id;
    int schedule_id;
    int row, col;
    int price;
    sale_type_t status;
} ticket_t;

typedef struct tag_sale {
    long id;
    char user[40];
    int ticket_schedule;
    int ticket_row, ticket_col;
    date_t date;
    times_t time;
    int value;
    sale_type_t type;
} sale_t;

typedef struct tag_sale_analysis {
    int id;
    play_t play;
    long totaltickets;
    long sales;
} sale_analysis_t;

#endif

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
#include"include/ttms_global.h"
#include"include/ttms_ui.h"
#include"include/ttms_srv.h"
#include<string.h>
#include<stdlib.h>
#include<time.h>

static int srv_sale_build_schedule_cmp(char *str, schedule_t *this) {
    int play = this->play_id;
    dstruct_linklist_link p = srv_find_play_id(play);
    if (p == NULL) {
        return -1;
    }
    play_t *hah = (play_t *)p->data;
    return strcmp(hah->name, str) == 0;
}

static int srv_sale_build_ticket_cmp(int id, ticket_t *this) {
    dstruct_linklist_link p = srv_find_schedule_id(this->schedule_id);
    schedule_t *hah = (schedule_t *)p->data;
    if (hah == NULL) {
        return 0;
    }
    return hah->id == id;
}


int srv_user_login(user_t loginmsg) {
    this_user = loginmsg;
    return RET_SUCCEED;
}

int srv_user_check(user_t *loginmsg) {
    dstruct_linklist_iter(g_user) {
        user_t *this = (user_t *)pi->data;
        if (strcmp(this->username, loginmsg->username) == 0) {
            if (strcmp(this->passwd, loginmsg->passwd) == 0) {
                loginmsg->type = this->type;
                return RET_SUCCEED;
            } else {
                return SRV_RET_LOGINFAILED;
            }
        }
    }
    return SRV_RET_LOGINFAILED;
}


int srv_sale_build_schedule(dstruct_linklist *schedules, char *question) {
    schedules->dll_init(schedules->this);

    dstruct_linklist_iter(g_schedule) {
        schedule_t *this = (schedule_t *)pi->data;
        if (srv_sale_build_schedule_cmp(question, this)) {
            schedules->append(schedules->this, (void *)this);
        }
    }
}

int srv_sale_build_ticket(dstruct_linklist *tickets, char *question) {
    int playid = -1;
    sscanf(question, "%d", &playid);
    tickets->dll_init(tickets->this);
    dstruct_linklist_iter(g_ticket) {
        ticket_t *this = (ticket_t *)pi->data;
        if (srv_sale_build_ticket_cmp(playid, this)) {
            tickets->append(tickets->this, (void *)this);
        }
    }
}

dstruct_linklist_link srv_find_seat_rc(int studio_id, int r, int c, int set) {
    dstruct_linklist_iter(g_seat) {
        seat_t *this = (seat_t *)pi->data;
        if (this->roomid == studio_id && this->row == r && this->col == c) {
            this->status = set;
            return pi;
        }
    }
    return NULL;
}

dstruct_linklist_link srv_find_seat(int studio_id, int r, int c) {
    dstruct_linklist_iter(g_seat) {
        seat_t *this = (seat_t *)pi->data;
        if (this->roomid == studio_id && this->row == r && this->col == c) {
            return pi;
        }
    }
    return NULL;
}

dstruct_linklist_link srv_find_studio_id(int id) {
    dstruct_linklist_iter(g_studio) {
        studio_t *this = (studio_t *)pi->data;
        if (this->id == id) {
            return pi;
        }
    }
    return NULL;
}

dstruct_linklist_link srv_find_schedule_id(int id) {
    dstruct_linklist_iter(g_schedule) {
        schedule_t *this = (schedule_t *)pi->data;
        if (this->id == id) {
            return pi;
        }
    }
    return NULL;
}

dstruct_linklist_link srv_find_play_id(int id) {
    dstruct_linklist_iter(g_play) {
        play_t *this = (play_t *)pi->data;
        if (this->id == id) {
            return pi;
        }
    }
    return NULL;
}

dstruct_linklist_link srv_find_user_name(char *str) {
    dstruct_linklist_iter(g_user) {
        user_t *this = (user_t *)pi->data;
        if (strcmp(str, this->username) == 0) {
            return pi;
        }
    }
    return NULL;
}

int srv_studio_equid(const void *va, const void *vb) {
    studio_t *a = (studio_t *)va,
        *b = (studio_t *)vb;
    return a->id == b->id;
}

int srv_user_equname(const void *va, const void *vb) {
    user_t *a = (user_t *)va;
    char *b = (char *)vb;
    return strcmp(a->username, b) == 0;
}

int srv_play_equid(const void *va, const void *vb) {
    play_t *a = (play_t *)va,
        *b = (play_t *)vb;
    return a->id == b->id;
}

int srv_studio_add(studio_t which) {
    g_studio.append(g_studio.this, (void *)&which);
    return srv_seat_studioadd(which.id, which.rows, which.cols);
}

int srv_play_add(play_t which) {
    g_play.append(g_play.this, (void *)&which);
    return 0;
}

int srv_user_add(user_t which) {
    g_user.append(g_user.this, (void *)&which);
    return 0;
}

int srv_seat_add(seat_t which) {
    g_seat.append(g_seat.this, (void *)&which);
    return 0;
}

int srv_build_seatarray(int roomid, int row, int col, int dest[row][col]) {
    memset(dest, 0, sizeof(int) * row * col);
    dstruct_linklist_iter(g_seat) {
        seat_t *this = (seat_t *)pi->data;
        if (this->roomid == roomid) {
            dest[this->row][this->col] = this->status;
        }
    }
}

int srv_seat_studioadd(int studio_id, int row, int col) {
    seat_t this;
    this.id = 1;
    this.roomid = studio_id;
    this.status = SEAT_GOOD;

    for (int i = 1; i <= row; i++) {
        for (int j = 1; j <= col; j++) {
            this.col = j;
            this.row = i;
            g_seat.append(g_seat.this, (void *)&this);
            this.id++;
        }
    }
    return 0;
}

int srv_seat_equroomid(const void *va, const void *vb) {
    seat_t *a = (seat_t *)va,
         *b = (seat_t *)vb;
    return a->roomid == b->roomid;
}

int srv_seat_studiodel(int studio_id) {
    seat_t c;
    c.roomid = studio_id;
    g_seat.delete(g_seat.this, (void *)&c, srv_seat_equroomid);
}

int srv_schedule_equid(const void *va, const void *vb) {
    schedule_t *a = (schedule_t *)va,
         *b = (schedule_t *)vb;
    return a->id == b->id;
}

int srv_schedule_add(schedule_t which) {
    g_schedule.append(g_schedule.this, (void *)&which);
    return srv_ticket_scheduleadd(which);
}

int srv_ticket_scheduleadd(schedule_t which) {
    studio_t *stu = (studio_t *)(srv_find_studio_id(which.studio_id)->data);
    ticket_t this;
    this.id = 1;
    this.schedule_id = which.id;
    this.status = 0; //0 not sale,1 saled
    play_t *ply = (play_t *)(srv_find_play_id(which.play_id)->data);
    this.price = ply->price;

    for (int i = 1; i <= stu->rows; i++) {
        for (int j = 1; j <= stu->cols; j++) {
            this.col = j;
            this.row = i;
            g_ticket.append(g_ticket.this, (void *)&this);
            this.id++;
        }
    }
    return 0;
}
int srv_ticket_scheduleid(const void *va, const void *vb) {
    schedule_t *a = (schedule_t *)va,
         *b = (schedule_t *)vb;
    return a->id == b->id;
}

int srv_ticket_scheduledel(int schedule_id) {
    ticket_t c;
    c.schedule_id = schedule_id;
    g_ticket.delete(g_ticket.this, (void *)&c, srv_ticket_scheduleid);
}

dstruct_linklist_link srv_find_ticket(int schedule_id, int row, int col) {
    dstruct_linklist_iter(g_ticket) {
        ticket_t *this = (ticket_t *)pi->data;
        if (this->schedule_id == schedule_id && this->row == row && this->col == col) {
            return pi;
        }
    }
    return NULL;
}


int srv_return_ticket(ticket_t *which) {
    sale_t record;
    sale_t *IDFINDER = (sale_t *)g_sale.dll_pend->data;
    if (IDFINDER == NULL) {
        record.id = 1;
    } else {
        record.id = IDFINDER->id + 1;
    }
    strcpy(record.user, this_user.username);
    record.ticket_schedule = which->schedule_id;
    record.ticket_row = which->row;
    record.ticket_col = which->col;
    record.date = srv_get_nowdate();
    record.time = srv_get_nowtime();
    record.value = which->price;
    record.type = SALE_RETURN;
    which->status = 0;
    g_sale.append(g_sale.this, (void *)&record);
    return 0;
}

int srv_sale_ticket(ticket_t *which) {
    sale_t record;
    sale_t *IDFINDER = (sale_t *)g_sale.dll_pend->data;
    if (IDFINDER == NULL) {
        record.id = 1;
    } else {
        record.id = IDFINDER->id + 1;
    }
    strcpy(record.user, this_user.username);
    record.ticket_schedule = which->schedule_id;
    record.ticket_row = which->row;
    record.ticket_col = which->col;
    record.date = srv_get_nowdate();
    record.time = srv_get_nowtime();
    record.value = which->price;
    record.type = SALE_SELL;
    which->status = 1;
    g_sale.append(g_sale.this, (void *)&record);
    return 0;
}


static int srv_sort_helper(const void *a, const void *b) {
    sale_analysis_t *va = (sale_analysis_t *)a,
        *vb = (sale_analysis_t *)b;
    return vb->sales - va->sales;
}

int srv_sort_saleanalysis() {
    sale_analysis_t SORT[1000];
    int i = 0;
    dstruct_linklist_iter(g_sale_analysis) {
        sale_analysis_t *this = (sale_analysis_t *)pi->data;
        SORT[i++] = *this;
    }
    qsort(SORT, i, sizeof(sale_analysis_t), srv_sort_helper);
    g_sale_analysis.dll_init(g_sale_analysis.this);
    for (int j = 0; j < i; j++) {
        g_sale_analysis.append(g_sale_analysis.this, (void *)&SORT[j]);
    }
}


int srv_build_saleanalysis() {
    g_sale_analysis.dll_init(g_sale_analysis.this);
    int id = 0;
    dstruct_linklist_iter(g_play) {
        play_t *this = (play_t *)pi->data;
        sale_analysis_t thisana;
        thisana.id = id++;
        thisana.play = *this;
        long cnt = 0;
        dstruct_linklist_iter(g_ticket) {
            ticket_t *tt = (ticket_t *)pi->data;
            schedule_t *tsc = (schedule_t *)srv_find_schedule_id(tt->schedule_id)->data;
            if (tsc->play_id == this->id && tt->status == 1) {
                cnt++;
            }
        }
        thisana.totaltickets = cnt;
        thisana.sales = cnt * this->price;
        g_sale_analysis.append(g_sale_analysis.this, (void *)&thisana);
    }
    return srv_sort_saleanalysis();
}

date_t srv_get_nowdate(void) {
    date_t result;
    time_t t;
    struct tm *lt;
    time (&t);
    lt = localtime (&t);
    result.year=lt->tm_year+1900;
    result.month=lt->tm_mon+1;
    result.day=lt->tm_mday;
    return result;
}

times_t srv_get_nowtime(void) {
    times_t result;
    time_t t;
    struct tm *lt;
    time(&t);
    lt = localtime(&t);
    result.hour=lt->tm_hour;
    result.min=lt->tm_min;
    result.sec=lt->tm_sec;
    return result;
}

int srv_indate(date_t a, date_t b, date_t c) {
	int rtn;
	if (a.year <= b.year) {
		if (c.year <= b.year&&c.year >= a.year) {
			if (c.month <= b.month&&c.month >= a.month) {
				if (c.day <= b.day&&c.day >= a.day)
					rtn = 0;
				else
                    rtn = 0;
			}
			else rtn = 0;
		}
		else
			rtn = 1;
	} else {
		if (c.year <= a.year&&c.year >= b.year) {
			if (c.month <= a.month&&c.month >= b.month) {
				if (c.day <= a.day&&c.day >= b.day)
					rtn = 0;
				else rtn = 0;
			}
			else rtn = 0;
		}
		else
			rtn = 1;
	}
	return rtn;
}

int srv_saler_acc(user_t *tu, date_t begin, date_t end) {
    int ret = 0;
    dstruct_linklist_iter(g_sale) {
        sale_t *this = (sale_t *)pi->data;
        if (strcmp(this->user, tu->username) == 0 && srv_indate(begin, end, this->date) == 0) {
            if (this->type == SALE_RETURN)
                ret -= this->value;
            else
                ret += this->value;
        }
    }
    return ret;
}

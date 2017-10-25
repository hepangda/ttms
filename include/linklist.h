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
#ifndef _TTMS_HEADER_LINKLIST
#define _TTMS_HEADER_LINKLIST

typedef struct tag_linknode_t {
    void *data;
    struct tag_linknode_t *prev, *next;
} linknode_t, *link_t;


typedef struct tag_linklist {
    link_t phead;
    link_t pend;
    unsigned int data_size;
} linklist_t;

typedef int (*oprfunc_t)(const void *, const void *);

void linklist_init(linklist_t *self);
link_t linklist_insert(linklist_t *self, void *data, link_t before);
link_t linklist_append(linklist_t *self, void *data);
link_t linklist_find(linklist_t *self, void *data, oprfunc_t operator_equal);
link_t linklist_delete(linklist_t *self, void *data, oprfunc_t operator_equal);

#define ll_init(self) linklist_init(&self)
#define ll_insert(self, data, before) linklist_insert(&self, (void *)&data, before)
#define ll_inserta(self, data, before) linklist_insert(&self, (void *)data, before)
#define ll_append(self, data) linklist_append(&self, (void *)&data)
#define ll_appenda(self, data) linklist_append(&self, (void *)data)
#define ll_find(self, data, opr) linklist_find(&self, (void *)&data, opr)
#define ll_finda(self, data, opr) linklist_find(&self, (void *)data, opr)
#define ll_delete(self, data, opr) linklist_delete(&self, (void *)&data, opr)
#define ll_deletea(self, data, opr) linklist_delete(&self, (void *)data, opr)

#define LINKLIST_CREATE(name, typename) \
    linklist_t name = { NULL, NULL, sizeof(typename)};

#define LINKLIST_CREATE_STATIC(name, typename) \
    static linklist_t name = { NULL, NULL, sizeof(typename)};

#define LINKLIST_ITER(name) \
    for (link_t pi = name.phead->next; pi != NULL; pi = pi->next)

#define LINKLIST_NEWLINK (link_t)malloc(sizeof(linknode_t))

#endif  //_TTMS_HEADER_LINKLIST

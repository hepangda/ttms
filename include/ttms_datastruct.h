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

    Author: Super Keshe Team
    E-mail: pangda@xiyoulinux.org
*************************************************************************/
#ifndef _TTMS_DATASTRUCT_H
#define _TTMS_DATASTRUCT_H

typedef struct tag_dstruct_linklist_node {
    void *data;
    struct tag_dstruct_linklist_node *prev, *next;
}dstruct_linklist_node, *dstruct_linklist_link;


typedef struct tag_dstruct_linklist {
    dstruct_linklist_link dll_phead;
    dstruct_linklist_link dll_pend;
    unsigned int dll_data_size;
    void (*dll_init)(struct tag_dstruct_linklist *this);

    struct tag_dstruct_linklist *this;

    dstruct_linklist_link
        (*append)(struct tag_dstruct_linklist *this,
                  void *data);

    dstruct_linklist_link
        (*insert)(struct tag_dstruct_linklist *this,
                  void *data,
                  dstruct_linklist_link before);

    dstruct_linklist_link
        (*find)(struct tag_dstruct_linklist *this,
                void *data,
                int (*operator_equal)(const void*, const void*));

    dstruct_linklist_link
        (*delete)(struct tag_dstruct_linklist *this,
                  void *data,
                  int (*operator_equal)(const void*, const void*));

}dstruct_linklist;


void dstruct_linklist_init(dstruct_linklist *this);
dstruct_linklist_link dstruct_linklist_insert
    (dstruct_linklist *this, void *data,
     dstruct_linklist_link before);
dstruct_linklist_link dstruct_linklist_append
    (dstruct_linklist *this, void *data);
dstruct_linklist_link dstruct_linklist_find
    (struct tag_dstruct_linklist *this, void *data,
     int (*operator_equal)(const void*, const void*));
dstruct_linklist_link dstruct_linklist_delete
    (dstruct_linklist *this, void *data,
     int (*operator_equal)(const void*, const void*));

#define datastruct_create_linklist(name, typename) \
    dstruct_linklist name = { NULL, NULL, \
                              sizeof(typename), \
                              dstruct_linklist_init, \
                              &name, \
                              dstruct_linklist_append,  \
                              dstruct_linklist_insert,  \
                              dstruct_linklist_find, \
                              dstruct_linklist_delete };

#define datastruct_create_static_linklist(name, typename) \
    static dstruct_linklist \
    name = { NULL, NULL,sizeof(typename),  \
             dstruct_linklist_init, &name, \
             dstruct_linklist_append,      \
             dstruct_linklist_insert,      \
             dstruct_linklist_find,        \
             dstruct_linklist_delete };

#define dstruct_linklist_iter(name) \
    for (dstruct_linklist_link pi = name.dll_phead->next; \
         pi != NULL; pi = pi->next)
#endif

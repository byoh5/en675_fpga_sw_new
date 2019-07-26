/*
 * Event loop based on select() loop
 * Copyright (c) 2002-2005, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#include "includes.h"
#include <stdio.h>
#include "wireless.h"
#include "common.h"
#include "eloop.h"
#include "ewl_wext.h" /*ewl_lib include file */
/**
    we don't have socket that is used in unix or other O/Ss
    The socket that we are using is a queue from shell command (wpa command)
    and It's number is '1'.

    Queue of eloop is handle event from WiFi drvier.
**/

struct eloop_sock {
    int sock;
    void *eloop_data;
    void *user_data;
    eloop_sock_handler handler;
};

struct eloop_timeout {
    struct os_time time;
    void *eloop_data;
    void *user_data;
    eloop_timeout_handler handler;
    struct eloop_timeout *next;
};


struct eloop_sock_table {
    int count;
    struct eloop_sock *table;
    int changed;
};

struct eloop_queue {
    void *eloop_data;
    void *user_data;
    eloop_queue_handler handler;
    struct eloop_queue *next;
};

struct eloop_data {    
    void *user_data;
    int max_sock;
    struct eloop_sock_table readers;
    struct eloop_timeout *timeout;
    struct eloop_queue   *queue;
    int terminate;
    int Reader_Table_Changed;
};

static struct eloop_data eloop;


int eloop_init(void)
{
    os_memset(&eloop, 0, sizeof(eloop));
    //eloop.user_data = user_data;
    return 0;
}


static int eloop_sock_table_add_sock(struct eloop_sock_table *table,
                                     int sock, eloop_sock_handler handler,
                                     void *eloop_data, void *user_data)
{
    struct eloop_sock *tmp;

    if (table == NULL)
        return -1;

    tmp = (struct eloop_sock *)
        os_realloc(table->table,
               (table->count + 1) * sizeof(struct eloop_sock));
    if (tmp == NULL)
        return -1;

    tmp[table->count].sock = sock;
    tmp[table->count].eloop_data = eloop_data;
    tmp[table->count].user_data = user_data;
    tmp[table->count].handler = handler;
    table->count++;
    table->table = tmp;
    if (sock > eloop.max_sock)
        eloop.max_sock = sock;
    table->changed = 1;

    return 0;
}


static void eloop_sock_table_remove_sock(struct eloop_sock_table *table,
                                         int sock)
{
	UNUSED(table);
	UNUSED(sock);
}


static void eloop_sock_table_dispatch(struct eloop_sock_table *table,
                      int *fds)
{
    int i;

    if (table == NULL || table->table == NULL)
        return;

    table->changed = 0;
    for (i = 0; i < table->count; i++) {
        table->table[i].handler(table->table[i].sock,
                        table->table[i].eloop_data,
                        table->table[i].user_data);
                        
        if (table->changed) break;
    }
	UNUSED(fds);
}

#if 0
static void eloop_sock_table_destroy(struct eloop_sock_table *table)
{
    if (table)
        os_free(table->table);
}
#endif

int eloop_register_read_sock(int sock, eloop_sock_handler handler,
                 void *eloop_data, void *user_data)
{

    return eloop_register_sock(sock, EVENT_TYPE_READ, handler,
                   eloop_data, user_data);
}


void eloop_unregister_read_sock(int sock)
{
    eloop_unregister_sock(sock, EVENT_TYPE_READ);
}


static struct eloop_sock_table *eloop_get_sock_table(eloop_event_type type)
{
    switch (type) {
    case EVENT_TYPE_READ:
        return &eloop.readers;
#if 0        
    case EVENT_TYPE_WRITE:
        return &eloop.writers;
    case EVENT_TYPE_EXCEPTION:
        return &eloop.exceptions;
#endif
	default:
		break;
    }

    return NULL;
}


int eloop_register_sock(int sock, eloop_event_type type,
            eloop_sock_handler handler,
            void *eloop_data, void *user_data)
{
    struct eloop_sock_table *table;

    table = eloop_get_sock_table(type);
    return eloop_sock_table_add_sock(table, sock, handler,
                     eloop_data, user_data);
}


void eloop_unregister_sock(int sock, eloop_event_type type)
{
    struct eloop_sock_table *table;

    table = eloop_get_sock_table(type);
    eloop_sock_table_remove_sock(table, sock);
}


int eloop_register_timeout(unsigned int secs, unsigned int usecs,
               eloop_timeout_handler handler,
               void *eloop_data, void *user_data)
{
    struct eloop_timeout *timeout, *tmp, *prev;
    
    timeout = os_malloc(sizeof(*timeout));
    
    if (timeout == NULL)
        return -1;
    
    os_get_time(&timeout->time);
    timeout->time.sec += secs;
    timeout->time.usec += usecs;
         
    while (timeout->time.usec >= 1000000) {
        timeout->time.sec++;
        timeout->time.usec -= 1000000;
    }

    timeout->eloop_data = eloop_data;
    timeout->user_data = user_data;
    timeout->handler = handler;
    timeout->next = NULL;
    
    if (eloop.timeout == NULL) {
        eloop.timeout = timeout;
        return 0;
    }
    
    prev = NULL;
    tmp = eloop.timeout;
    
    while (tmp != NULL) {
        if (os_time_before(&timeout->time, &tmp->time))
        break;
        prev = tmp;
        tmp = tmp->next;
    }

    if (prev == NULL) {
        timeout->next = eloop.timeout;
        eloop.timeout = timeout;
    } else {
        timeout->next = prev->next;
        prev->next = timeout;
    }

    return 0;
}


int eloop_cancel_timeout(eloop_timeout_handler handler,
             void *eloop_data, void *user_data)
{
    struct eloop_timeout *timeout, *prev, *next;
    int removed = 0;

    prev = NULL;
    timeout = eloop.timeout;
    while (timeout != NULL) {
        next = timeout->next;

        if (timeout->handler == handler &&
            (timeout->eloop_data == eloop_data ||
             eloop_data == ELOOP_ALL_CTX) &&
            (timeout->user_data == user_data ||
             user_data == ELOOP_ALL_CTX)) {
            if (prev == NULL)
                eloop.timeout = next;
            else
                prev->next = next;
            os_free(timeout);
            removed++;
        } else
            prev = timeout;

        timeout = next;
    }

    return removed;
}


/*
    
*/
void eloop_event_q_dispatch(void)
{
    struct iw_event  *event;
            
    if(ewl_wireless_recv_event(&event))
    {
        if(eloop.queue)
        {
            /*This handler is wpa_driver_wexit_init, 
                 eloop_data -> wpa_driver_wext_data,
                 user_data ->  struct wpa_supplicant*/
            eloop.queue->handler(event,
                                eloop.queue->eloop_data,
                                eloop.queue->user_data); 
         }
    }
    
    if(event)
        os_free(event);                 
}

/* 
    This function should be polled every 10m sec. 
    We can't use task of OS.
*/
void eloop_run(void)
{
//    int res;
//    struct timeval _tv;
    struct os_time now; // tv
    
    /* handle event from moal_recv_event() */    
    eloop_event_q_dispatch();
    
    /* handle commands from shell*/
    eloop_sock_table_dispatch(&eloop.readers, NULL);
    
    /*supplicant req's timeouts */
    if (eloop.timeout) {
        struct eloop_timeout *tmp;
        os_get_time(&now);
        if (!os_time_before(&now, &eloop.timeout->time)) {
            tmp = eloop.timeout;
            eloop.timeout = eloop.timeout->next;
            tmp->handler(tmp->eloop_data, tmp->user_data);
            os_free(tmp);
        }
    }
}


void eloop_terminate(void)
{
    eloop.terminate = 1;
}


void eloop_destroy(void)
{
    struct eloop_timeout *timeout, *prev;

    timeout = eloop.timeout;
    while (timeout != NULL) {
        prev = timeout;
        timeout = timeout->next;
        os_free(prev);
    }
}


int eloop_terminated(void)
{
    return eloop.terminate;
}


void eloop_wait_for_read_sock(int sock)
{
	UNUSED(sock);
}


void * eloop_get_user_data(void)
{
    return eloop.user_data;
}

/*
    we just use one queue for wext interface
*/

void eloop_register_queue(eloop_queue_handler handler, void *eloop_data, void *user_data)
{
    struct eloop_queue *queue;
    
    queue = os_malloc(sizeof(*queue));
    if (queue == NULL)
        return;
    
    queue->eloop_data = eloop_data;
    queue->user_data = user_data;
    queue->handler = handler;
    queue->next = NULL;
    
    if(eloop.queue == NULL)
    {
        eloop.queue = queue;
        return;
    }

    //add new item to queue     
    eloop.queue->next = queue;
}
#include <stdlib.h>
#include <stdio.h>
#include "doubly_linked_list.h"

struct doubly_linked_list_t* dll_create()
{
    struct doubly_linked_list_t *temp = calloc(1, sizeof(struct doubly_linked_list_t));
    if(!temp) return NULL;

    return temp;
}

int dll_push_back(struct doubly_linked_list_t* dll, int value)
{
    if(!dll) return 1;
    struct node_t *temp = calloc(1, sizeof(struct node_t));
    if(!temp) return 2;

    temp->data = value;
    if(!dll_size(dll))
    {
        dll->head = dll->tail = temp;
        temp->next = NULL;
        temp->prev = NULL;
    }
    else
    {
        dll->tail->next = temp;
        temp->next = NULL;
        temp->prev = dll->tail;
        dll->tail = temp;
    }

    return 0;
}
int dll_push_front(struct doubly_linked_list_t* dll, int value)
{
    if(!dll) return 1;
    struct node_t *temp = calloc(1, sizeof(struct node_t));
    if(!temp) return 2;

    temp->data = value;
    if(!dll_size(dll))
    {
        dll->head = dll->tail = temp;
        temp->next = NULL;
        temp->prev = NULL;
    }
    else
    {
        dll->head->prev = temp;
        temp->next = dll->head;
        dll->head = temp;
    }

    return 0;
}
int dll_pop_front(struct doubly_linked_list_t* dll, int *err_code)
{
    if(!dll || !dll_size(dll))
    {
        if(err_code) *err_code = 1;
        return -1;
    }

    int ret = dll->head->data;
    if(dll_size(dll) == 1)
    {
        free(dll->head);
        dll->tail = NULL;
        dll->head = NULL;
    }
    else
    {
        struct node_t *temp = dll->head;
        dll->head = dll->head->next;
        dll->head->prev = NULL;
        free(temp);
    }

    if(err_code) *err_code = 0;
    return ret;
}
int dll_pop_back(struct doubly_linked_list_t* dll, int *err_code)
{
    if(!dll || !dll_size(dll))
    {
        if(err_code) *err_code = 1;
        return -1;
    }

    int ret = dll->tail->data;
    if(dll_size(dll) == 1)
    {
        free(dll->tail);
        dll->head = NULL;
        dll->tail = NULL;
    }
    else
    {
        struct node_t *temp = dll->tail;
        dll->tail = dll->tail->prev;
        dll->tail->next = NULL;
        free(temp);
    }

    if(err_code) *err_code = 0;
    return ret;
}

int dll_back(const struct doubly_linked_list_t* dll, int *err_code)
{
    if(!dll || !dll_size(dll))
    {
        if(err_code) *err_code = 1;
        return -1;
    }

    if(err_code) *err_code = 0;
    return dll->tail->data;
}
int dll_front(const struct doubly_linked_list_t* dll, int *err_code)
{
    if(!dll || !dll_size(dll))
    {
        if(err_code) *err_code = 1;
        return -1;
    }

    if(err_code) *err_code = 0;
    return dll->head->data;
}

struct node_t* dll_begin(struct doubly_linked_list_t* dll)
{
    if(!dll || !dll_size(dll)) return NULL;

    return dll->head;
}
struct node_t* dll_end(struct doubly_linked_list_t* dll)
{
    if(!dll || !dll_size(dll)) return NULL;

    return dll->tail;
}
int dll_size(const struct doubly_linked_list_t* dll)
{
    if(!dll) return -1;

    int size = 0;
    struct node_t *temp = dll->head;

    while(temp)
    {
        ++size;
        temp = temp->next;
    }

    return size;
}
int dll_is_empty(const struct doubly_linked_list_t* dll)
{
    if(!dll) return -1;
    else if(!dll_size(dll)) return 1;
    return 0;
}

int dll_at(const struct doubly_linked_list_t* dll, unsigned int index, int *err_code)
{
    if(!dll || !dll_size(dll) || index >= (unsigned)dll_size(dll))
    {
        if(err_code) *err_code = 1;
        return -1;
    }
    struct node_t *temp = dll->head;
    unsigned i = 0;

    while(i < index)
    {
        ++i;
        temp = temp->next;
    }

    if(err_code) *err_code = 0;
    return temp->data;
}

int dll_insert(struct doubly_linked_list_t* dll, unsigned int index, int value)
{
    if(!dll || (!dll_size(dll) && (index != 0)) || index > (unsigned)dll_size(dll)) return 1;

    struct node_t *new = calloc(1, sizeof(struct node_t));
    if(!new) return 2;
    new->data = value;

    if(!index)
    {
        new->prev = NULL;
        new->next = dll->head;
        if(dll->head)  dll->head->prev = new;
        if(!dll_size(dll)) dll->tail = new;
        dll->head = new;
    }
    else if((int)index == dll_size(dll))
    {
        new->prev = dll->tail;
        new->next = NULL;
        if(dll->tail) dll->tail->next = new;
        dll->tail = new;
    }
    else
    {
        unsigned i = 0;
        struct node_t *temp = dll->head;
        while((i+1) < index)
        {
            temp = temp->next;
            ++i;
        }

        struct node_t *next = temp->next;
        temp->next = new;
        new->prev = temp;
        new->next = next;
        next->prev = new;
    }


    return 0;
}
int dll_remove(struct doubly_linked_list_t* dll, unsigned int index, int *err_code)
{
    if(!dll || !dll_size(dll) || index >= (unsigned)dll_size(dll))
    {
        if(err_code) *err_code = 1;
        return -1;
    }

    int ret;
    if(!index)
    {
        ret = dll->head->data;
        struct node_t *temp = dll->head;
        dll->head = dll->head->next;
        if(dll->head) dll->head->prev = NULL;
        free(temp);
    }
    else if((int)index == (dll_size(dll)-1))
    {
        ret = dll->tail->data;
        struct node_t *temp = dll->tail;
        dll->tail = dll->tail->prev;
        if(dll->tail) dll->tail->next = NULL;
        free(temp);
    }
    else
    {
        unsigned i = 0;
        struct node_t *temp = dll->head;
        while(i < index)
        {
            temp = temp->next;
            ++i;
        }

        ret = temp->data;
        struct node_t *prev = temp->prev;
        struct node_t *next = temp->next;
        prev->next = next;
        next->prev = prev;
        free(temp);
    }

    if(err_code) *err_code = 0;
    return ret;
}

void dll_clear(struct doubly_linked_list_t* dll)
{
    if(!dll || !dll_size(dll)) return;
    struct node_t *temp = dll->head;
    struct node_t *temp1;

    while(temp)
    {
        temp1 = temp;
        temp = temp->next;
        free(temp1);
    }
    dll->head = NULL;
    dll->tail = NULL;
}

void dll_display(const struct doubly_linked_list_t* dll)
{
    if(!dll || !dll_size(dll)) return;
    struct node_t *temp = dll->head;

    while(temp)
    {
        printf("%d ",temp->data);
        temp = temp->next;
    }
    printf("\n");
}
void dll_display_reverse(const struct doubly_linked_list_t* dll)
{
    if(!dll || !dll_size(dll)) return;
    struct node_t *temp = dll->tail;

    while(temp)
    {
        printf("%d ",temp->data);
        temp = temp->prev;
    }
    printf("\n");
}
#ifndef LIST_H
#define LIST_H

#include "HT_info.h"
#include <stdlib.h>
#include <string.h>

//list that contains pointers to hash files' infos
typedef struct HtNode
{
    HT_info * info;
    struct HtNode * next;
}
HtNode;


typedef struct HtList
{
    HtNode * head;
}
HtList;

int ListInsert(HtList*, HT_info*);
int ListRemove(HtList*, char*);
int ListDestroy(HtList*);


#endif

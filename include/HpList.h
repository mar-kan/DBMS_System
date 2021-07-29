#ifndef LIST_H
#define LIST_H

#include "HP_info.h"
#include <stdlib.h>
#include <string.h>

//list that contains pointers to heap files' infos

typedef struct HpNode
{
    HP_info * info;
    struct HpNode * next;
}
HpNode;


typedef struct HpList
{
    HpNode * head;
}
HpList;

int ListInsert(HpList*, HP_info*);
int ListRemove(HpList*, char*);
int ListDestroy(HpList*);


#endif

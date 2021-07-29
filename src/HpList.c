#include "../include/HpList.h"
#include<stdio.h>


int ListInsert(HpList * list, HP_info * hpInfo)
{
    if (!list)
    {
        list = malloc(sizeof(HpList));
        list->head = NULL;
    }

    //creates a list node with given HP_info
    HpNode * node = malloc(sizeof(HpNode));
    node->info = hpInfo;
    node->next = NULL;

    //adds new node at the beginning of the list for speed
    if (list->head)
        node->next = list->head;
    list->head = node;

    return 0;
}


int ListRemove(HpList * list, char * info_id)
{
    if (!list || !list->head)
        return -1;

    HpNode * temp, * node = list->head;
    while (node)
    {
        if (strcmp(node->info->attrName, info_id) == 0)
        {
            temp->next = node->next;
            free(node);
            return 0;
        }
        temp = node;    //stores previous node
        node = node->next;
    }

    return 0;
}


int ListDestroy(HpList * list)
{
    if (!list || !list->head)
        return -1;

    HpNode * temp, * node;
    temp = list->head;
    while(temp)
    {
        node = temp;
        temp = temp->next;

        //free(node->info); //destroyed by HP_CloseFile
        free(node);
    }
    return 0;
}
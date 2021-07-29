#include "../include/HtList.h"

int ListInsert(HtList * list, HT_info * hpInfo)
{
    if (!list)
        list->head = malloc(sizeof(HtList));

    //creates a list node with given HP_info
    HtNode * node = malloc(sizeof(HtNode));
    node->info = hpInfo;
    node->next = NULL;
    //adds new node at the beginning of the list for speed
    if (list->head)
        node->next = list->head;
    list->head = node;

    return 0;
}


int ListRemove(HtList * list, char * info_id)
{
    if (!list || !list->head)
        return -1;

    HtNode * temp, * node = list->head;
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


int ListDestroy(HtList * list)
{
    if (!list || !list->head)
        return -1;

    HtNode * temp, * node;
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
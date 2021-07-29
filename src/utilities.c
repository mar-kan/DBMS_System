#include <stdio.h>
#include "../include/utilities.h"


int createRecordFromLine(char * line, Record * record)
{
    int iteration=0;
    char * temp = (char*)malloc(sizeof(line));

    int i=1;                                    //ignores 1st char of line because its a bracket ({)
    while (line[i] != '}')  //last character
    {
        //stores each value from line in temp
        int j=0;
        while (line[i] != ',')
        {
            if (line[i] == '"') //ignores quotation marks
            {
                i++;
                continue;
            }
            if (line[i] == '}')
                break;

            temp[j++] = line[i++];
        }
        temp[j+1] = '\0';

        //initializes each variable of record
        switch (iteration)
        {
            case 0:
                record->id = atoi(temp);
                break;
            case 1:
                strcpy(record->name, temp);
                break;
            case 2:
                strcpy(record->surname, temp);
                break;
            case 3:
                strcpy(record->address, temp);
                break;
            default:
                break;
        }
        if (line[i] == '}')
            break;

        iteration++;
        i++;
    }
    free(temp);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "../include/HpList.h"
#include "../include/utilities.h"
#include "../include/HP.h"

#define MAX_RECORDS 1500
#define FILENAME "file"


int main(int argc, char** argv)
{
    char filename [5];

    BF_Init();
    HpList * files = malloc(sizeof(HpList)); //list that includes all files that have been created and opened
    Record * record = malloc(sizeof(Record));

    strcpy(filename, FILENAME);

    //requests and opens a file with records
    printf("Please input file path\n");
    char input[50];
    scanf("%s",input);

    FILE * fp = fopen(input, "r");
    if (!fp)
    {
        printf("Error in opening record file\n");
        exit(-1);
    }

    chdir("heap_files");    //changes dir to create block files in their own dir

    //creates and opens an empty block file with its name as id
    printf("Creating file: %s\n\n", filename);
    if (HP_CreateFile(filename, 'i', filename, sizeof(filename)) < 0)
        exit(EXIT_FAILURE);

    printf("Opening file\n");
    HP_info * info;
    if (!(info = HP_OpenFile(filename)))
        exit(EXIT_FAILURE);
    ListInsert(files, info);    //inserts info in list

    //reads file line by line and inserts each record
    size_t size = 0;
    char * line = NULL;
    printf("\nInserting all records\n");
    int count=0;
    while (getline(&line, &size, fp) != EOF)
    {
        //creates record with each line read
        if (createRecordFromLine(line, record) < 0)
        {
            printf("Error in creating record\n");
            continue;
        }

        //when a file reaches its max records creates new file and opens that for use
        if (count == MAX_RECORDS)
        {
            //creates and opens an empty block file with its name as id
            filename[0]++;
            printf("Creating file: %s\n", filename);
            if (HP_CreateFile(filename, 'i', filename, sizeof(filename)) < 0)
                exit(EXIT_FAILURE);

            printf("Opening file\n");
            if (!(info = HP_OpenFile(filename)))
                exit(EXIT_FAILURE);
            ListInsert(files, info);    //inserts info in list

            count=0;    //resets count for each file
        }

        if (HP_InsertEntry(*info, *record) < 0)
        {
            printf("Error in insertion of key %d\n",record->id);
            break;
        }
        count++;
    }
    //picked a random key that exists in all input files
    srand(time(NULL));
    int value = rand() % 1000;  //gets random int that exists in all datasets
    char * key = malloc(10);
    sprintf(key, "%d", value);

    //searches in all block files and prints every entry found with key = given id
    printf("\nGetting all entries\n");
    HpNode * temp = files->head;
    while (temp)
    {
        int blkCnt;

        if ((blkCnt = HP_GetAllEntries(*temp->info, key)) < 0)
            printf("Getting all entries failed\n");
        else
            printf("Read %d blocks\n", blkCnt);

        temp = temp->next;
    }

    //searches in all block files and deletes record with given id
    printf("\nDeleting a record\n");
    temp = files->head;
    while (temp)
    {
        if (HP_DeleteEntry(*(temp)->info, key) < 0)
            printf("Delete failed\n");

        temp = temp->next;
    }

    //searches in all block files and prints every entry found with key = given id
    printf("\nGetting all entries\n");
    temp = files->head;
    while (temp)
    {
        int blkCnt;

        if ((blkCnt = HP_GetAllEntries(*temp->info, key)) < 0)
            printf("Getting all entries failed\n");
        else
            printf("Read %d blocks\n", blkCnt);

        temp = temp->next;
    }

    //closes and deletes all opened files
    printf("\nClosing files\n");
    temp = files->head;
    while (temp)
    {
        if (HP_CloseFile(temp->info) < 0)
            exit(EXIT_FAILURE);

        temp = temp->next;
    }

    //frees all memory
    fclose(fp);
    ListDestroy(files);
    free(line);
    free(record);
    free(files);
    free(key);

    printf("Exiting\n");
    return 0;
}


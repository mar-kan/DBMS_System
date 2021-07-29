#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "../include/HtList.h"
#include "../include/utilities.h"
#include "../include/HT.h"

#define MAX_RECORDS 1500
#define FILENAME "file0"



int main(int argc, char** argv)
{

    if (argc < 2)
    {
        printf("Please type the input file\n");
        exit(-1);
    }
    char filename [6];

    BF_Init();
    HtList * files = malloc(sizeof(HtList)); //list that includes all files that have been created and opened
    Record * record = malloc(sizeof(Record));

    strcpy(filename, FILENAME);

    //requests and opens a file with records
    char input[50];
    strcpy(input, argv[1]);

    FILE * fp = fopen(input, "r");
    if (!fp)
    {
        printf("Error in opening record file\n");
        exit(-1);
    }

    chdir("hash_files");    //changes dir to create block files in their own dir

    //creates and opens an empty block file with its name as id
    printf("Creating file: %s\n", filename);
    if (HT_CreateIndex(filename, 'i', filename, sizeof(filename), 50) < 0)
        exit(EXIT_FAILURE);

    printf("Opening file\n");
    HT_info * info;
    if (!(info = HT_OpenIndex(filename)))
        exit(EXIT_FAILURE);
    ListInsert(files, info);    //inserts info in list

    //reads file line by line and inserts each record
    size_t size = 0;
    char * line = NULL;
    int count=0;

    printf("Inserting all records\n");
    while (getline(&line, &size, fp) != EOF)
    {
        //creates record with each line read
        createRecordFromLine(line, record);

        //when a file reaches its max records creates new file and opens that for use
        if (count == MAX_RECORDS)
        {
            //creates and opens an empty block file with its name as id
            filename[0]++;
            printf("Creating file: %s\n", filename);
            if (HT_CreateIndex(filename, 'i', filename, sizeof(filename), 40) < 0) {
                exit(EXIT_FAILURE);
            }

            printf("Opening file\n");
            if (!(info = HT_OpenIndex(filename))) {
                exit(EXIT_FAILURE);
            }
            ListInsert(files, info);    //inserts info in list
            count=0;    //resets count for each file
        }

        if (HT_InsertEntry(*info, *record) < 0)
            break;

        count++;
    }

    //picked a random key that exists in all input files
    srand(time(NULL));
    int value = rand() % 1000;  //gets random int that exists in all datasets
    char * key = malloc(10);
    sprintf(key, "%d", value);

    //searches in all block files and prints every entry found with key = given id
    printf("\nGetting all entries\n");
    HtNode * temp = files->head;
    while (temp)
    {
        int blkCnt;
        if ((blkCnt = HT_GetAllEntries(*temp->info, key)) < 0)
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
        if (HT_DeleteEntry(*(temp)->info, key) < 0)
            printf("Delete failed\n");

        temp = temp->next;
    }

    //searches in all block files and prints every entry found with key = given id
    printf("\nGetting all entries\n");
    temp = files->head;
    while (temp)
    {
        int blkCnt;
        if ((blkCnt = HT_GetAllEntries(*temp->info, key)) < 0)
            printf("Getting all entries failed\n");
        else
            printf("Read %d blocks\n", blkCnt);

        temp = temp->next;
    }

    //prints hash statistics for every file
    /*temp = files->head;
    while (temp)
    {
        if (HashStatistics(temp->info->attrName) < 0)
            printf("Error in calculating hash statistics\n");

        temp = temp->next;
    }*/

    //closes and deletes all opened files
    printf("\nClosing files\n");
    temp = files->head;
    while (temp)
    {
        if (HT_CloseIndex(temp->info) < 0)
            exit(EXIT_FAILURE);

        temp = temp->next;
    }

    //frees all memory
    fclose(fp);
    ListDestroy(files);
    free(line);
    free(record);
    free(files);

    printf("Exiting\n");
    return 0;
}


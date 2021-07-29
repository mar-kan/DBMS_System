#include <stdlib.h>
#include "../include/HP.h"


int HP_CreateFile(char *fileName, char attrType, char* attrName, int attrLength)    //creates heap file and writes its id 'H' and its number of records (always 0 at first)
{
    int fd;
    void* block = NULL;
    int blkCnt = 0;
    char heap_id = 'H';

    //creates and opens new block file
    if (BF_CreateFile(fileName) < 0)
    {
        BF_PrintError("Error creating BF file");
        return -1;
    }
    if ((fd = BF_OpenFile(fileName)) < 0)
    {
        BF_PrintError("Error opening BF file");
        return -1;
    }

    //allocates its 1st block
    if (BF_AllocateBlock(fd) < 0)
    {
        BF_PrintError("Error allocating BF file");
        return -1;
    }

    if (BF_ReadBlock(fd, blkCnt, &block) < 0)
    {
        BF_PrintError("Error reading BF file");
        return -1;
    }

    //writes heap id to block
    memcpy(block,&heap_id,sizeof(char));

    if (BF_WriteBlock(fd, blkCnt) < 0)
    {
        BF_PrintError("Error writing BF file");
        return -1;
    }

    //closes file
    if (BF_CloseFile(fd) < 0)
    {
        BF_PrintError("Error closing block file");
        return -1;
    }
    return 0;
}


HP_info* HP_OpenFile(char *fileName /* όνομα αρχείου */)   //opens heap file, creates its info checks that it is a heap file and returns it
{
    int fd;
    void * block;
    if ((fd = BF_OpenFile(fileName)) < 0)
    {
        BF_PrintError("Error opening block file");
        return NULL;
    }

    //reads data
    if (BF_ReadBlock(fd, 0, &block) < 0) //reads file's block 0 and stores its data to block
    {
        BF_PrintError("Error reading block file");
        return NULL;
    }

    //gets id of file
    char file_id;
    memcpy (&file_id, block, sizeof(char));

    //checks if opened file is heap file
    if (file_id != 'H')
    {
        printf("File is not a heap file\n");
        return NULL;
    }

    //creates file's HP_info
    HP_info * info = malloc(sizeof(HP_info));
    info->fileDesc = fd;
    info->attrName = fileName;
    info->attrType = 'c';
    info->attrLength = sizeof (fileName);

    //closes file
    if (BF_CloseFile(fd) < 0)
    {
        BF_PrintError("Error closing block file");
        return NULL;
    }

    return info;
}


int HP_CloseFile(HP_info* header_info)    //closes heap file and deletes its info
{
    //opens requested file
    if ((BF_OpenFile(header_info->attrName)) < 0)
    {
        BF_PrintError("Error opening block file");
        return -1;
    }

    if (BF_CloseFile(header_info->fileDesc) < 0)
    {
        BF_PrintError("Error closing block file");
        return -1;
    }

    //frees HP_info memory
    free(header_info);
    return 0;
}


int HP_InsertEntry(HP_info header_info, Record record) //inserts a new entry in a block file
{                                                      //allocates new blocks in file if necessary
    void* block = NULL;
    int blkCnt;
    int recordNum = 0;

    if ((BF_OpenFile(header_info.attrName)) < 0)
    {
        BF_PrintError("Error opening block file");
        return -1;
    }

    //gets last block
    if((blkCnt = BF_GetBlockCounter(header_info.fileDesc)) < 0)
    {
        BF_PrintError("Error getting block counter");
        return -1;
    }
    blkCnt--;

    if(!blkCnt)   //only 1st info block
    {
        //allocates a new block
        if (BF_AllocateBlock(header_info.fileDesc) < 0)
        {
            BF_PrintError("Error allocating block");
            return -1;
        }
        blkCnt++;
    }

    //reads last block and gets its number of records
    if (BF_ReadBlock(header_info.fileDesc, blkCnt, &block) < 0)
    {
        BF_PrintError("Error in reading block");
        return -1;
    }
    memcpy (&recordNum, block, sizeof(int));

    //checks if there is enough space for the new record
    if (sizeof(int) + recordNum*sizeof(Record) + sizeof(Record) > BLOCK_SIZE)   //no space
    {
        //allocates a new block
        if (BF_AllocateBlock(header_info.fileDesc) < 0)
        {
            BF_PrintError("Error allocating block");
            return -1;
        }
        blkCnt++;
        recordNum=0;    //sets its records to 0

        //reads newly allocated block
        if (BF_ReadBlock(header_info.fileDesc, blkCnt, &block) < 0)
        {
            BF_PrintError("Error reading block");
            return -1;
        }
    }

    //updates recordNum
    recordNum++;
    memcpy(block, &recordNum, sizeof(int));

    //inserts record at the end of the block
    memcpy(block+sizeof(int)+recordNum*sizeof(Record), &record, sizeof(Record));

    //writes block to block file
    if (BF_WriteBlock(header_info.fileDesc, blkCnt) < 0)
    {
        BF_PrintError("Error writing block");
        return -1;
    }

    //closes file
    if (BF_CloseFile(header_info.fileDesc) < 0)
    {
        BF_PrintError("Error closing block file");
        return -1;
    }

    return 0;
}


int HP_DeleteEntry( HP_info header_info, /* επικεφαλίδα του αρχείου*/ void *value /* τιμή τουπεδίου-κλειδιού προς διαγραφή */)
{
    void* block = NULL;
    int blkCnt;
    int recordNum = 0;

    //opens requested file
    if ((BF_OpenFile(header_info.attrName)) < 0)
    {
        BF_PrintError("Error opening block file");
        return -1;
    }

    //gets block count
    if((blkCnt = BF_GetBlockCounter(header_info.fileDesc)) < 0)
    {
        BF_PrintError("Error getting block counter");
        return -1;
    }

    //searches each block of the file for record with id = value
    for (int i=0; i<blkCnt; i++)
    {
        //reads each block
        if (BF_ReadBlock(header_info.fileDesc, i, &block) < 0)
        {
            BF_PrintError("Error in reading block");
            return -1;
        }

        //checks file id and skips 1st id block
        char file_id;
        if (i==0)
        {
            memcpy (&file_id, block, sizeof(char));

            //checks if opened file is heap file
            if (file_id != 'H')
            {
                printf("File is not a heap file\n");
                return -1;
            }
            continue;
        }

        //gets num of records
        memcpy (&recordNum, block, sizeof(int));

        //checks each record
        for (int j=0; j<=recordNum; j++)
        {
            //gets current record and compares its key to value
            Record * rec = malloc(sizeof(Record));
            memcpy(rec, block+sizeof(int) + j*sizeof(Record), sizeof(Record));

            //converts record id to string
            char * id = (char*)malloc(sizeof(int));
            sprintf(id, "%d", rec->id);

            if (strcmp(id, (char*)value) == 0)  //record found
            {
                //reformats block file and overwrites record to de deleted
                for (int k=j; k<recordNum-1; k++)
                {
                    //brings every record of block <sizeof(Record)> bytes forward
                    memcpy(block+sizeof(int) + k*sizeof(Record), block+sizeof(int) + (k+1)*sizeof(Record), sizeof(Record));
                }
                recordNum--; //decreases block's records

                //writes new block
                if (BF_WriteBlock(header_info.fileDesc, i) < 0)
                {
                    BF_PrintError("Error in writing new block");
                    free(rec);
                    free(id);
                    return -1;
                }
                printf("Record with key %s deleted successfully\n", (char *)value);
                free(rec);
                free(id);
                return 0;
            }
            free(id);
            free(rec);
        }
    }

    //closes file
    if (BF_CloseFile(header_info.fileDesc) < 0)
    {
        BF_PrintError("Error closing block file");
        return -1;
    }

    printf("Record key %s not found\n", (char*)value);
    return 0;
}


int HP_GetAllEntries( HP_info header_info, /* επικεφαλίδα του αρχείου */ void *value /* τιμή τουπεδίου-κλειδιού προς αναζήτηση */)
{
    void* block = NULL;
    int blkCnt;
    int recordNum;
    int flag = 0;

    //opens requested file
    if ((BF_OpenFile(header_info.attrName)) < 0)
    {
        BF_PrintError("Error opening block file");
        return -1;
    }

    //gets block count
    if((blkCnt = BF_GetBlockCounter(header_info.fileDesc)) < 0)
    {
        BF_PrintError("Error getting block counter");
        return -1;
    }

    //searches each block of the file for record with id = value
    for (int i=0; i<blkCnt; i++)
    {
        //reads each block
        if (BF_ReadBlock(header_info.fileDesc, i, &block) < 0)
        {
            BF_PrintError("Error in reading block");
            return -1;
        }

        //checks file id and skips 1st id block
        char file_id;
        if (i==0)
        {
            memcpy (&file_id, block, sizeof(char));

            //checks if opened file is heap file
            if (file_id != 'H')
            {
                printf("File is not a heap file\n");
                return -1;
            }
            continue;
        }

        //gets num of records
        memcpy (&recordNum, block, sizeof(int));

        //checks each record
        for (int j=0; j<=recordNum; j++)
        {
            //gets current record and compares its key to value
            Record * rec = malloc(sizeof(Record));
            memcpy(rec, block+sizeof(int) + j*sizeof(Record), sizeof(Record));

            //converts record id to string
            char * id = (char*)malloc(sizeof(int));
            sprintf(id, "%d", rec->id);

            if (strcmp(id, (char*)value) == 0)  //record found
            {
                //prints record
                printf("Found Record with key: %d\n"
                       "Full name: %s %s\n"
                       "Address: %s\n", rec->id, rec->name, rec->surname, rec->address);

                flag = 1;
            }
            free(id);
            free(rec);
        }
    }

    if (!flag)
        printf("Record key %s not found\n", (char*)value);

    //closes file
    if (BF_CloseFile(header_info.fileDesc) < 0)
    {
        BF_PrintError("Error closing block file");
        return -1;
    }

    return blkCnt;  //returns num of blocks read
}
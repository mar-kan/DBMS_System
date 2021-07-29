#ifndef HP_H
#define HP_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BF.h"
#include "Record.h"
#include "HpList.h"


int HP_CreateFile(char *fileName, /* όνομα αρχείου */char attrType, /* τύπος πεδίου-κλειδιού: 'c', 'i' */char* attrName, /* όνομα πεδίου-κλειδιού */int attrLength /* μήκος πεδίου-κλειδιού */);
HP_info* HP_OpenFile(char *fileName /* όνομα αρχείου */ );
int HP_CloseFile( HP_info* header_info );
int HP_InsertEntry( HP_info header_info, /* επικεφαλίδα του αρχείου*/ Record record /* δομή πουπροσδιορίζει την εγγραφή */ );
int HP_DeleteEntry( HP_info header_info, /* επικεφαλίδα του αρχείου*/ void *value /* τιμή τουπεδίου-κλειδιού προς διαγραφή */);
int HP_GetAllEntries( HP_info header_info, /* επικεφαλίδα του αρχείου */ void *value /* τιμή τουπεδίου-κλειδιού προς αναζήτηση */);


#endif

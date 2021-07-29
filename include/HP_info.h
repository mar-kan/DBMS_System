#ifndef HP_INFO_H
#define HP_INFO_H

typedef struct
{
    int fileDesc; /* αναγνωριστικός αριθμός ανοίγματος αρχείου από το επίπεδο block */
    char attrType; /* ο τύπος του πεδίου που είναι κλειδί για το συγκεκριμένο αρχείο,'c'ή'i' */
    char*attrName; /* το όνομα του πεδίου που είναι κλειδί για το συγκεκριμένο αρχείο */
    int attrLength; /* το μέγεθος του πεδίου που είναι κλειδί για το συγκεκριμένο αρχείο */
}
HP_info;


#endif
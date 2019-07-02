/* scriptDB.h */

#ifndef _SCRIPT_DB_H_
#define _SCRIPT_DB_H_

#include <dirent.h>


typedef struct scriptDB {
    DIR *dbDir;
    int numOfScripts;
    /* Path to database */
    char *path;
} scriptDB;

#endif /* _SCRIPT_DB_H_ */

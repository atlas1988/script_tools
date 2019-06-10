#include <stdio.h>
#include <sqlite3.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
	printf("%s = %s\n", azColName[2], argv[2] ? argv[2] : "NULL");
//   for(i=0; i<argc; i++){
//     printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
//   }
   printf("\n");
   return 0;
}

int main(){
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	int fd_tmp;
	char *sql;
	  /* Create SQL statement */
   sql = "CREATE TABLE COMPANY("  \
         "ID INT PRIMARY KEY     NOT NULL," \
         "NAME           TEXT    NOT NULL," \
         "AGE            INT     NOT NULL," \
         "ADDRESS        CHAR(50)," \
         "SALARY         REAL );";

   rc = sqlite3_open("/data/mac.db3", &db);
   if( rc ){
      printf("Can't open database: %s\n", sqlite3_errmsg(db));
      return -1;
   }else{
      printf("Opened database successfully\n");
   }
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   	printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      printf("Table created successfully\n");
   }
     /* Create SQL statement */
   sql = "SELECT * from OUI WHERE MAC='00-0A-4D'";
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
      printf("SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      printf("Records created successfully\n");
   }
   sqlite3_close(db);
	return 0;
}

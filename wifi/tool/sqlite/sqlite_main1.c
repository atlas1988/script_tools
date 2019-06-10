#include <stdio.h>
#include <sqlite3.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
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

	// create file
	fd_tmp = open("/data/test.db",O_RDWR|O_CREAT,0666);
	close(fd_tmp);
   rc = sqlite3_open("/data/test.db", &db);

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
   sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
         "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
         "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

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

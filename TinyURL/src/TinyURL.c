/*
 ============================================================================
 Name : TinyURL.c
 Author : Tharindu Madushanka
 Version :
 Copyright : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <openssl/sha.h>
static int callback(void *data, int argc, char **argv, char **azColName) {
// this function for print the database retrivals.
	int i;
	if(argv[0]==NULL)printf("empty");
	printf("%s\n :", argv[0]);
	/*
	 * if you want to print all the retrivals from query
	 for (i = 0; i < argc; i++) {
	 printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	 }*/
	return 0;
}
static char* hash_generator(char *word) {
//this function for create the hash value for string
	int i = 0;
	unsigned char temp[SHA_DIGEST_LENGTH];
	char buf[SHA_DIGEST_LENGTH * 2];
	memset(buf, 0x0, SHA_DIGEST_LENGTH * 2);
	memset(temp, 0x0, SHA_DIGEST_LENGTH);
//hash generate using SHA1 algorithm
	SHA1((unsigned char *) word, strlen(word), temp);
	for (i = 0; i < 4; i++) {
		sprintf((char*) &(buf[i]), "%02x", temp[i]);
	}
	printf("hash for %s is : %s\n",word,buf);
	return buf;
}
/*
 static void show(char *hash_value){
 }*/
int main(int argc, char* argv[]) {
	char *hash = "12346";
	char *URL = "www.w3school.com";
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	const char* data;

	hash_generator(URL);

// open sqlite3 database "URL_Records"
	rc = sqlite3_open("URL_Records.db", &db);
// if db exist it will open otherwise create a db
	if (rc) {
		fprintf(stderr, "can't open");
		exit(0);
	}
//query to create "Records" table if not exists.
	sql = "CREATE TABLE if not exists Records("
			"Hash CHAR(5) PRIMARY KEY NOT NULL,"
			"URL TEXT NOT NULL);";
	rc = sqlite3_exec(db, sql, callback, (void*) data, &zErrMsg);
//Execute the sql statement.
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	char sql_1[255] = "SELECT URL from Records where Hash = ";
	strcat(sql_1, hash);
	sql = sql_1;
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
//Execute the sql statement.
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "operation successfully\n");
	}
	sqlite3_close(db);

}

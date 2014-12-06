/*
 ============================================================================
 Name        : TinyURL.c
 Author      : Tharindu  Madushanka
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <openssl/sha.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

static char* hash_generator(char *word) {
	int i = 0;
	unsigned char temp[SHA_DIGEST_LENGTH];
	char buf[SHA_DIGEST_LENGTH * 2];
	//char *word = "tharigdddfndu";

	memset(buf, 0x0, SHA_DIGEST_LENGTH * 2);
	memset(temp, 0x0, SHA_DIGEST_LENGTH);

	SHA1((unsigned char *) word, strlen(word), temp);

	for (i = 0; i < 4; i++) {
		sprintf((char*) &(buf[i]), "%02x", temp[i]);
	}
	return buf;

}

int main(int argc, char* argv[]) {

	char *URL = "WWW.GOOGLE.COM";
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	const char *string = "The quick brown fox jumped over the lazy dog's back";

	// open sqlite3 database "URL_Records"
	rc = sqlite3_open("URL_Records.db", &db);

	// if db exist it will open otherwise create a db
	if (rc) {
		fprintf(stderr, "can't open");
		exit(0);
	} else {
		fprintf(stdout, "opened");
	}

	//query to create "Records" table if not exists.
	sql = "CREATE TABLE if not exists Records("
			"Id INTEGER PRIMARY KEY  AUTOINCREMENT,"
			"URL           TEXT    NOT NULL,"
			"Hash          CHAR(5)     NOT NULL);";
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	//Execute the sql statement.
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Table created successfully\n");
	}

	sql = "SELECT * from Records";

	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	//Execute the sql statement.
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "operation successfully\n");
	}
	sqlite3_close(db);

	// hash

	printf("SHA1 is %s\n", hash_generator("tharirrndu"));

}

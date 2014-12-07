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


void quit(){
	exit(1);
}

static int callback(void *data, int argc, char **argv, char **azColName) {
// this function for print the database retrivals.
	printf("hiiiiii");
	printf("%s\n :", argv[0]);
	/*
	 * if you want to print all the retrivals from query
	 * int i;
	 for (i = 0; i < argc; i++) {
	 printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	 }*/
	return 0;
}
char* hash_generator(char *word) {
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

	char* out_hash = malloc(strlen(buf) + 1);
	strcpy(out_hash, buf);

	return out_hash;
}
/*
 static void show(char *hash_value){
 }*/
int main(int argc, char* argv[]) {

	char hash_in[255], URL_in[255];
	char *hash, *URL;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	const char* data;

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
			fprintf(stderr, "SQL error1: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}

while(1){
	int a;
	printf("Enter 1 to add URl , 2 to hash and other to quit :");

	printf("got the number: %d\n", scanf("%d", &a));

	if (a == 1) {
		// insert URL
		printf("Enter the URL :");
		scanf("%s", &URL_in);
		URL = URL_in;
		hash = hash_generator(URL);
		//strcpy(hash, hash_generator(URL));

		printf("the tiny url for that : www.myURl/%s\n", hash);

		char sql_in_1[255] = "INSERT OR IGNORE INTO Records(Hash,URL) VALUES ('";
		strcat(sql_in_1, hash);
		strcat(sql_in_1, "'");
		strcat(sql_in_1, ",'");
		strcat(sql_in_1, URL);
		strcat(sql_in_1, "');");

		//printf("done %s\n ", sql_in_1);

		rc = sqlite3_exec(db, sql_in_1, callback, 0, &zErrMsg);
		//Execute the sql statement.
			if (rc != SQLITE_OK) {
				fprintf(stderr, "SQL error2: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
			} else {
				fprintf(stdout, "database updated\n");
			}

	} else if (a == 2) {
		printf("Enter the Hash :");
		scanf("%s", &hash_in);
		hash = hash_in;

		char sql_1[255] = "SELECT URL from Records where Hash = '";
			strcat(sql_1, hash);
			strcat(sql_1, "';");
			sql = sql_1;

			rc = sqlite3_exec(db, sql_1, callback, 0, &zErrMsg);
					//Execute the sql statement.
						if (rc != SQLITE_OK) {
							fprintf(stderr, "SQL error2: %s\n", zErrMsg);
							sqlite3_free(zErrMsg);
						} else {
							fprintf(stdout, "URL is : ");
						}
	}else {
		break;
	}

	}
	sqlite3_close(db);
	quit();
}

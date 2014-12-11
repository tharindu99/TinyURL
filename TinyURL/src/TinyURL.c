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

int query_stat = 0;
char *result = NULL;

char hash_in[255], URL_in[255];
char *hash, *URL;
sqlite3 *db;
char *zErrMsg = 0;
int rc;
char *sql;
const char* data;
char *longUrl, *shortUrl;
char *baseUrl = "www.tinyURL.com/";

void quit() { //quit from system
	printf("quit !!");
	exit(1);
}

static int callback(void *data, int argc, char **argv, char **azColName) {
// this function can use for print the database retrivals.
	query_stat = argc;
	result = argv[0];
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

void shorten(char *longUrl, char *shortUrl) {

	char sql_chkURL[255] = "SELECT Hash from Records where URL = '";
	strcat(sql_chkURL, longUrl);
	strcat(sql_chkURL, "';");
	//printf("%s\n", sql_chkURL);


	char *con_url[255];
	strcat(con_url,baseUrl);
	hash = hash_generator(longUrl);

	rc = sqlite3_exec(db, sql_chkURL, callback, 0, &zErrMsg);
	//printf("%d", query_stat);
	if (query_stat != 0) { //Url is on database
		shortUrl = result;
		query_stat = 0;
		result = NULL;

		strcat(con_url,shortUrl);
		//printf("the tiny url for that :%s\n",shortUrl);

	} else { //url not in database
		//collition resolving and hash generating
		while (1) {
			sql_chkURL[255] = "SELECT Hash from Records where Hash = '";
			strcat(sql_chkURL, hash);
			strcat(sql_chkURL, "';");
			rc = sqlite3_exec(db, sql_chkURL, callback, 0, &zErrMsg);
			if (query_stat == 0)
				break;
			hash = hash_generator(hash);
		}
		query_stat = 0;
		result = NULL;
		strcat(con_url,hash);

		char sql_in_1[255] = "INSERT OR IGNORE INTO Records(Hash,URL) VALUES ('";
		strcat(sql_in_1, hash);
		strcat(sql_in_1, "'");
		strcat(sql_in_1, ",'");
		strcat(sql_in_1, longUrl);
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
	}
	//assign result to shortUrl
	shortUrl = con_url;
	printf("The tiny url  : %s\n",shortUrl);
}

void getLongUrl(char *shortUrl, char *longUrl) {
	// this shortUrl = saved hash of longurl and it may not included base Url
	char sql_getLong[255] = "SELECT URL from Records where Hash = '";
	strcat(sql_getLong, shortUrl);
	strcat(sql_getLong, "';");

	rc = sqlite3_exec(db, sql_getLong, callback, 0, &zErrMsg);
	//Execute the sql statement.
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error2: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	longUrl = result;
	printf("long url %s,\n", longUrl);
	result = NULL;
	query_stat = 0;
}

int main(int argc, char* argv[]) {

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
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	//Execute the sql statement.
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error1: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	while (1) {
		int a;
		printf("Enter 1 to add URl , 2 to hash and other to quit :");

		scanf("%d", &a);

		if (a == 1) {
			// insert URL
			printf("Enter the URL :");
			scanf("%s", URL_in);
			shorten(&URL_in[0], NULL);

		} else if (a == 2) {
			printf("Enter the Hash :");
			scanf("%s", hash_in);
			getLongUrl(&hash_in[0], NULL);
		} else {
			break;
		}

	}
	sqlite3_close(db);
	quit();
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mysql.h"
#include "app_mysql.h"

static MYSQL my_connection;
static int dbconnected = 0;

static int get_artist_id(char *artist);

int database_start(char *name, char *pwd) {

	if (dbconnected) return 1;
	
	mysql_init(&my_connection);
	if (!mysql_real_connect(&my_connection, "localhost", name, pwd "blpcd", 0, NULL, 0)) {
		fprintf(stderr, "Database connection failure: %d, %s\n", mysql_errno(&my_connection), mysql_error(&my_connection));
		return 0;
	}
	dbconnected = 1;
	return 1;
} /* database_start */


void database_end() {
	if (dbconnected) mysql_close(&my_connection);
	dbconnected = 0;
} /* database_end */


int add_cd(char *artist, char *title, char *catalogue, int *cd_id) {
	
	MYSQL_RES *res_ptr;
	MYSQL_ROW mysqlrow;

	int res;
	char is[250];
	char es[250];
	int artist_id = -1;
	int new_cd_id = -1;

	if (!dbconnected) return 0;
	
	artist_id = get_artist_id(artist);

	mysql_escape_string(es, title, strlen(title));
	sprintf(is, "INSERT INTO cd(title, artist_id, catalogue) VALUES('%s', %d, '%s')", es, artist_id, catalogue);
	res = mysql_query(&my_connection, is);
	if (res) {
		fprintf(stderr, "Insert error %d: %s\n", mysql_errno(&my_connection), mysql_error(&my_connection));
		return 0;
	}

	res = mysql_query(&my_connection, "SELECT LAST_INSERT_ID()");
	if (res) {
		printf("SELECT error: %s\n", mysql_error(&my_connection));
		return 0;
	} else {
		res_ptr = mysql_store_result(&my_connection);
		if (res_ptr) {
			if (mysql_num_rows(res_ptr) > 0) {
		if (mysqlrow = mysql_fetch_row(res_ptr)) {
			sccanf(mysqlrow[0], "%d", &artist_id);
		}
			}
			mysql_free_result(res_ptr);
		}
	}
	if (artist_id != -1) return artist_id;

	/* 

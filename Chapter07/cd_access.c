#define _XOPEN_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include <ndbm.h>
#include "cd_data.h"

#define CDC_FILE_BASE "cdc_data"
#define CDT_FILE_BASE "cdt_data"
#define CDC_FILE_DIR "cdc_data.dir"
#define CDC_FILE_PAG "cdc_data.pag"
#define CDT_FILE_DIR "cdt_data.dir"
#define CDT_FILE_PAG "cdc_data.pag"

static DBM *cdc_dbm_ptr = NULL;
static DBM *cdt_dbm_ptr = NULL;

int database_initialize(const int new_database)
{
	int open_mode = O_CREAT | O_RDWR;

	/* Close existing databases if they are open. */
	if (cdc_dbm_ptr) dbm_close(cdc_dbm_ptr);
	if (cdt_dbm_ptr) dbm_close(cdt_dbm_ptr);

	if (new_database) {
		/* Delete exist files */
		(void) unlink(CDC_FILE_PAG);
		(void) unlink(CDC_FILE_DIR);
		(void) unlink(CDT_FILE_PAG);
		(void) unlink(CDT_FILE_DIR);
	}

	/* Open database (create if it doesn't exist) */
	cdc_dbm_ptr = dbm_open(CDC_FILE_BASE, open_mode, 0644);
	cdt_dbm_ptr = dbm_open(CDT_FILE_BASE, open_mode, 0644);

	if (!cdc_dbm_ptr || !cdt_dbm_ptr) {
		fprintf(stderr, "Unable to create database\n");
		cdc_dbm_ptr = cdt_dbm_ptr = NULL;
		return(0);
	}
	return(1);
}

void database_close(void)
{
	if (cdc_dbm_ptr) dbm_close(cdc_dbm_ptr);
	if (cdt_dbm_ptr) dbm_close(cdt_dbm_ptr);

	cdc_dbm_ptr = cdt_dbm_ptr = NULL;
}

cdc_entry get_cdc_entry(const char *cd_catalog_ptr)
{
	cdc_entry entry_to_return;
	char entry_to_find[CAT_CAT_LEN + 1];
	datum local_data_datum;
	datum local_key_datum;

	memset(&entry_to_return, '\0', sizeof(entry_to_return));

	if (!cdc_dbm_ptr || !cdt_dbm_ptr) return (entry_to_return);
	if (!cd_catalog_ptr) return (entry_to_return);
	if (strlen(cd_catalog_ptr) >= CAT_CAT_LEN) return (entry_to_return);

	memset(&entry_to_find, '\0', sizeof(entry_to_find));
	strcpy(entry_to_find, cd_catalog_ptr);

	local_key_datum.dptr = (void *) entry_to_find;
	local_key_datum.dsize = sizeof(entry_to_find);

	memset(&local_data_datum, '\0', sizeof(local_data_datum));
	local_data_datum = dbm_fetch(cdc_dbm_ptr, local_key_datum);
	if (local_data_datum.dptr) {
		memcpy(&entry_to_return, (char *)local_data_datum.dptr, local_data_datum.dsize);
	}
	return (entry_to_return);
} /* get_cdc_entry */

cdt_entry get_cdt_entry(const char *cd_catalog_ptr, const int track_no)
{
	cdt_entry entry_to_return;
	char entry_to_find[CAT_CAT_LEN + 10];
	datum local_data_datum;
	datum local_key_datum;

	memset(&entry_to_return, '\0', sizeof(entry_to_return));

	if (!cdc_dbm_ptr || !cdt_dbm_ptr) return (entry_to_return);
	if (!cd_catalog_ptr) return (entry_to_return);
	if (strlen(cd_catalog_ptr) >= CAT_CAT_LEN) return (entry_to_return);
	/* Create a search key composed of the catalog entry and the track number. */
	memset(&entry_to_find, '\0', sizeof(entry_to_find));
	sprintf(entry_to_find, "%s %d", cd_catalog_ptr, track_no);

	local_key_datum.dptr = (void *) entry_to_find;
	local_key_datum.dsize = sizeof(entry_to_find);

	memset(&local_data_datum, '\0', sizeof(local_data_datum));
	local_data_datum = dbm_fetch(cdt_dbm_ptr, local_key_datum);
	if (local_data_datum.dptr) {
		memcpy(&entry_to_return, (char *) local_data_datum.dptr, local_data_datum.dsize);
	}
	return (entry_to_return);
}

int add_cdc_entry(const cdc_entry entry_to_add)
{
	char key_to_add[CAT_CAT_LEN + 1];


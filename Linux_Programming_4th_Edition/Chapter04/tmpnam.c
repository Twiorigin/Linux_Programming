#include <stdio.h>
#include <stdlib.h>

int main()
{
	char tmpname[L_tmpnam];
	char *filename;
	FILE *tmpfp;
	char *template = "/tmp/mytmpfile.xxxxxx";

	int fd = mkstemp(template);

	if (fd == -1) {
		perror("mkstemp fail");
		exit(1);
	}


	
	printf("file path: %s\n", template);
	
	filename = tmpnam(tmpname);

	printf("Temporary file name is: %s\n", filename);
	tmpfp = tmpfile();
	if(tmpfp)
		printf("Opened a temporary file OK\n");
	else
		perror("tmpfile");
	exit(0);
}

#include <stdio.h>
#include <stdlib.h>

int main()
{
	char s[256];
	int n;
	float f;
	char c;

	scanf("Hello,%d,%g,%c, %[^\n]", &n,&f,&c,s);
	printf("%d %g %c %s\n", n,f,c,s);  	

	exit(0);
}

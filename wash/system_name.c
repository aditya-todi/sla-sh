#include <stdio.h>
#include <sys/utsname.h>
#include <unistd.h>

int main()
{
	struct utsname abc;
	uname(&abc);
	char hostname[100];
	gethostname(hostname, 100);
	printf("%s\n", abc.sysname);
	printf("%s\n", hostname);
	return 0;
}
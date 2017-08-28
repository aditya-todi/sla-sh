#include <stdio.h>
#include <string.h>

int main ()
{
	char str[] ="cd ..; ls -a; cd Assignment;";
	char * p;
	char commands[100][100];
	int i=0;
	for (char *p = strtok(str,";"); p != NULL; p = strtok(NULL, ";"), i++)
	{
		int j=0;
		int len=strlen(p);
		for(j=0; j<len; j++)
		{
			commands[i][j]=p[j];
		}
	}
	int noOfCommands=i;
	for(i=0; i<noOfCommands; i++)
	{
		for(char *p=strtok(commands[i], " "); p!=NULL; p=strtok(NULL, " "))
			puts(p);
		printf("----------------\n");
	}
	return 0;
}
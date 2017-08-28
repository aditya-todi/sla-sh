#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
	char d[100];
	char e[100];
	char home[13]="/home/shovan";
	getcwd(d, 100);
	
	int i, counter=0;
	for(i=0; i<11; i++)
	{
		if(d[i]==home[i])
		{
			counter++;
		}
	}
	if(counter==11)
	{
		e[0]=126;
		for(i=1; i<=strlen(d); i++)
		{
			e[i]=d[i+11];
		}
	printf("%s\n", e);
	}
	else
	{
		printf("%s\n", d);
	}

	return 0;
}
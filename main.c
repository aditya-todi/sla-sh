#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

void slashloop();
void print_prompt();
void read_command();
void split_command(char* str);
int slash_cd(char **args);
int slash_pwd(char **args);
int slash_echo(char **args, int noOfArgs);
void slash_ls(char **args);
void slash_send(char *args);
void slash_execute(char** args);

char mainFilePath[100];

int slash_cd(char **args)
{
	if(args[1]==NULL)
	{
		chdir(mainFilePath);
	}
	else
	{
		if(args[1][0]==126)
		{
			chdir(mainFilePath);
		}
		else if(chdir(args[1])!=0)
		{
			printf("Error in going to specified location\n");
		}
	}
	return 1;
}

int slash_pwd(char **args)
{
	if(args[1])
	{
		printf("Too many arguments\n");
	}
	else
	{
		char cwd[100];
		if(getcwd(cwd, 100))
		{
			printf("%s\n", cwd);
		}
		else
		{
			printf("Error in printing present working directory");
		}
	}
}

int slash_echo(char **args, int noOfArgs)
{
	int i;
	for(i=1; i<noOfArgs; i++)
	{
		printf("%s ", args[i]);
	}
	printf("\n");
}

void slash_ls(char **args)
{
	char cwd[100];
	getcwd(cwd, 100);
	DIR *currDir;
	struct dirent *currFile;
	currDir=opendir(cwd);
	struct stat fileDetails={0};

	if(args[1]==NULL) //only ls
	{
		if(currDir!=NULL)
		{
			while(currFile=readdir(currDir))
			{
				if(currFile->d_name[0]!=46)
				{
					printf("%s\n", currFile->d_name);
				}
			}
		}
	}

	else if(args[1][1]==97 && args[1][2]==0) //only ls -a
	{
		if(currDir!=NULL)
		{
			while(currFile=readdir(currDir))
			{
				printf("%s\n", currFile->d_name);
			}
		}	
	}

	else if(args[1][1]==108) //only ls -l
	{
		printf("Permissions      Size Date        Name\n");
		printf("---------------------------------------\n");
		if(currDir!=NULL)
		{
			while(currFile=readdir(currDir))
			{
				getcwd(cwd, 100);
				strcat(cwd, "/");
				strcat(cwd, currFile->d_name);
				stat(cwd, &fileDetails);
				int permissions=fileDetails.st_mode;
				int i;
				char perm[10];
				char yes[10]={"rwxrwxrwx"};
				for(i=0; i<9; i++)
				{
					if((permissions>>(9-i-1))&1)
					{
						perm[i+1]=yes[i];
					}
					else
					{
						perm[i+1]='-';
					}
				}
				if(permissions>>15)
				{
					perm[0]='-';
				}
				else
				{
					perm[0]='d';
				}
				i=10;
				struct tm *time;
				char date[100];
				time=localtime(&fileDetails.st_mtime);
				strftime(date, 100, "%m %d %H:%M", time);
				printf("%s ", perm);
				printf("%*ld ", i,fileDetails.st_size);
				printf("%s ", date);
				printf("%s\n", currFile->d_name);	
			}
		}
	}
}

void slash_pinfo(char** args)
{
	int i=0;
	while(args[i]!=NULL)
		i++;

	if(i==2)
	{
		char statPath[100]={"/proc/"};
		strcat(statPath, args[1]);
		strcat(statPath, "/stat");
		int fd_file1;
		fd_file1=open(statPath, O_RDONLY);
		if(fd_file1>0)
		{
			char buf[1000];
			read(fd_file1, buf, 1000);
			char* arr[100];
			int i=0;
			for(i=0; i<100; i++)
				arr[i]=NULL;

			i=0;
			for(char *p=strtok(buf, " "); p!=NULL; p=strtok(NULL, " "), i++)
			{
				arr[i]=p;
			}

			printf("PID: %s\nProcess Status: %s\nVirtual Memory: %s\nFile Name: %s\n", arr[0], arr[2], arr[25], arr[1]);
		}
		else
		{
			printf("Error in finding process, make sure this pid exists\n");
		}
	}
	else
	{
		int pid=getpid();
		char* argu[2];
		argu[0]=args[0];
		argu[1]=malloc(1000);
		sprintf(argu[1], "%d", pid);
		slash_pinfo(argu);
	}
}

void slash_nightswatch(char** args)
{
	char* options=args[1];
	char* command=args[2];
	printf("%s %s\n", options, command);
	// while(1)
	// {
	char sleep[10];
	slash_send(sleep);
		// printf("LOL\n");		
	// }
}

void slash_exec(char **args)
{
	// pid_t parent;
	pid_t pid1;
	pid_t pid2;
	int len=strlen(args[0]);
	pid1=fork();
	if(args[0][len-1]!='&')
	{
		if(pid1==0) //child
		{
			if(execvp(args[0], args)==-1)
			{
				printf("%s: Command not found\n", args[0]);
			}
		}
		else if(pid1<0)
		{
			printf("Error in forking\n");
		}
		else //parent
		{
			wait(NULL);
			// exit(0);
		}
	}
	else
	{
		if(pid1==0)
		{
			pid2=fork();
			if(pid2==0) //grandchild
			{
				pid_t child=getppid();
				args[0][len-1]=0;
				// setpgid(0, 0);
				printf("%d STARTED\n", child);
				execvp(args[0], args);
			}
			else if(pid2!=0) //child
			{
				wait(NULL);
				printf("%d DONE\n", pid2);
				exit(0);
			}
		}
	}
}


void slashloop()
{
	int isError=0;
	while(!isError)
	{
		print_prompt();
		read_command();
	}
}

void print_prompt()
{
	char* username;
	username=getpwuid(geteuid())->pw_name;
	
	char hostname[1000];
	gethostname(hostname, 1000);

	char cwd[100];
	getcwd(cwd, 100);
	
	int len=strlen(mainFilePath);
	char e[100];
	
	int i, counter=0;
	for(i=0; i<len; i++)
	{
		if(cwd[i]==mainFilePath[i])
		{
			counter++;
		}
	}
	if(counter==len)
	{
		e[0]=126;
		for(i=1; i<=strlen(cwd); i++)
		{
			e[i]=cwd[i+len-1];
		}
	printf("\033[1;32m<%s@%s:\033[1;34m%s\033[1;32m>\033[0m$ ", username, hostname, e);
	}
	else
	{
		printf("\033[1;32m<%s@%s:\033[1;34m%s\033[1;32m>\033[0m$ ", username, hostname, cwd);
	}
}

void read_command()
{
	char input[100];
	int i;
	for(i=0; i<100; i++)
	{
		input[i]=0;
	}
	if(fgets(input, 100, stdin))
	{
		int len=strlen(input);
		input[len-1]=0; //replaces the last "\n" with a "\0"
		split_command(input);
	}
	else
	{
		//
		printf("\n");
	}
}

void split_command(char* str)
{
	char * p;
	char commands[100][100];
	int i=0;
	int j=0;
	for(i=0; i<100; i++)
		for(j=0; j<100; j++)
			commands[i][j]=0;
	i=0;
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
	for(j=0; j<noOfCommands; j++)
	{
		slash_send(commands[j]);
	}
}

void slash_send(char *args)
{
	char* arr[100];
	int i=0;
	for(i=0; i<100; i++)
		arr[i]=NULL;

	i=0;
	for(char *p=strtok(args, " "); p!=NULL; p=strtok(NULL, " "), i++)
	{
		arr[i]=p;
	}

	slash_execute(arr);
}

void slash_execute(char** args)
{
	if(!strcmp(args[0], "cd"))
	{
		slash_cd(args);
	}
	else if(!strcmp(args[0], "pwd"))
	{
		slash_pwd(args);
	}
	else if(!strcmp(args[0], "echo"))
	{
		int i;
		for(i=0; args[i]!=NULL; i++);
		slash_echo(args, i);
	}
	else if(!strcmp(args[0], "ls"))
	{
		slash_ls(args);
	}
	else if(!strcmp(args[0], "pinfo"))
	{
		slash_pinfo(args);
	}
	else if(!strcmp(args[0], "nightswatch"))
	{
		slash_nightswatch(args);
	}
	else
	{
		slash_exec(args);
	}
}

int main()
{
	getcwd(mainFilePath, 100);
	slashloop();
	return 0;
}
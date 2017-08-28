#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char str[100];
    fgets(str, 100, stdin);
    int len=strlen(str);
    str[len-1]=0;
    printf("You typed '%s'", str);
    return(0);
}


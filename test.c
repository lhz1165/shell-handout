
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int parseline(const char *cmdline, char **argv);
int main()
{   
    char *cmdline="quit -l -s &\n";
    char *argv[128];
    
    int bg = parseline(cmdline, argv);
     printf("bg job?= %d\n",bg);
   
    
    //printf("%s\n",argv[0]);
    for (int i = 0; argv[i] != NULL; i++) {
        printf("%s\n", argv[i]);
    }
    return 0;
}

/* Misc manifest constants */
#define MAXLINE    1024 

int parseline(const char *cmdline, char **argv) 
{
    static char array[MAXLINE]; /* holds local copy of command line */
    char *buf = array;          /* ptr that traverses command line */
    char *delim;                /* points to first space delimiter */
    int argc;                   /* number of args */
    int bg;                     /* background job? */

    strcpy(buf, cmdline);
    buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* ignore leading spaces */
	buf++;

    /* Build the argv list */
    argc = 0;
    if (*buf == '\'') {
	buf++;
	delim = strchr(buf, '\'');
    }
    else {
	delim = strchr(buf, ' ');
    }

    while (delim) {
	argv[argc++] = buf;
	*delim = '\0';
	buf = delim + 1;
	while (*buf && (*buf == ' ')) /* ignore spaces */
	       buf++;

	if (*buf == '\'') {
	    buf++;
	    delim = strchr(buf, '\'');
	}
	else {
	    delim = strchr(buf, ' ');
	}
    }
    argv[argc] = NULL;
    
    if (argc == 0)  /* ignore blank line */
	return 1;

    /* should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0) {
	argv[--argc] = NULL;
    }
    return bg;
}
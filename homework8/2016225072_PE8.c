#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1

int main()
{
    FILE *fp;
    FILE *popen2(const char *command, const char *mode);
    char buf[BUFSIZ];

    fp = popen2("ls", "r");

    while(fgets(buf, BUFSIZ, fp) != NULL)
        fputs(buf, stdout);

    return 0;
}

FILE *popen2(const char *command, const char *mode)
{
    FILE *fp;
    int pid, thepipe[2];

    if(pipe(thepipe) == -1)
    {
        perror("Cannot get a pipe");
        exit(1);
    }

    if((pid = fork()) == -1)
    {
        perror("Cannot fork");
        exit(2);
    }

    if(strcmp(mode, "r") == 0)
    {
        if(pid > 0)
        {
            close(thepipe[1]);
            fp = fdopen(thepipe[0], "r");
            return fp;
        }
        else
        {
            close(thepipe[0]);
            if(dup2(thepipe[1], 1) == -1)
            {
                perror("Cannot dup2");
                exit(3);
            }
            close(thepipe[1]);
            execl("/bin/sh", "sh", "-c", command, NULL);
        }
    }
    else if(strcmp(mode, "w") == 0)
    {
        if(pid > 0)
        {
            close(thepipe[0]);
            fp = fdopen(thepipe[0], "w");
            return fp;
        }
        else
        {
            close(thepipe[1]);
            if(dup2(thepipe[0], 0) == -1)
            {
                perror("Cannot dup2");
                exit(3);
            }
            close(thepipe[0]);
            execl("/bin/sh", "sh", "-c", command, NULL);
        }
    }
}

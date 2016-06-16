#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define oops(m,x) {perror(m); exit(x);}

int main(int ac, char **av)
{
    int pipeAB[2], pipeBC[2], pid;

    if(ac != 4) {
        fprintf(stderr, "usage : pipe cmd1 cmd2 cmd3\n");
        exit(1);
    }
    
    if(pipe(pipeAB) == -1)
        oops("Cannot get a pipeAB", 1);

    if((pid = fork()) == -1)
        oops("Cannot forkAB", 2);

    //forkAB Parent
    if(pid > 0) {
        close(pipeAB[1]);
        if(dup2(pipeAB[0], 0) == -1)
            oops("could not redirect pipeAB stdin", 3);
        close(pipeAB[0]);
        execlp(av[3], av[3], NULL);
        oops(av[3], 3);
    }
    //forkAB Child
    close(pipeAB[0]);
    if(dup2(pipeAB[1], 1) == -1)
        oops("could not redirect pipeBC stdout", 4);
    close(pipeAB[1]);


    if(pipe(pipeBC) == -1)
        oops("Cannot get a pipeBC", 1);

    if((pid = fork()) == -1)
        oops("Cannot fork", 2);

    if(pid > 0) {
        close(pipeBC[1]);
        if(dup2(pipeBC[0], 0) == -1)
            oops("could not redirect pipeBC stdin", 3);
        close(pipeBC[0]);
        execlp(av[2], av[2], NULL);
        oops(av[2], 4);
    }

    close(pipeBC[0]);
    if(dup2(pipeBC[1], 1) == -1)
        oops("could not redirect pipeBC stdout", 4);
    close(pipeBC[1]);

    execlp(av[1], av[1], NULL);
    oops(av[1], 3);
}


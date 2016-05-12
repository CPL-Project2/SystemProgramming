#include <stdio.h>
#include <signal.h>

int main(void)
{
    void f(int);
    int i;

    signal(SIGINT, f);
    for(i = 0; i < 5; i ++) {

        printf("hello\n");
        sleep(1);
    }
    return 0;
}

void f(int signum)
{
    char answer;
    printf("Interrupted! OK to quit (y/n)?");
    __fpurge(stdin);
    scanf("%c", &answer);

    if(answer=='y') exit(1);
}


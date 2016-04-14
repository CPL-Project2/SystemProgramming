#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    struct stat info;

    if(argc < 3)
    {
        printf("Usage : ./StudentID_PE3 file1 file2(or directory)\n");
        return 0;
    }
    else if(!strcmp(argv[1], argv[2]))
    {
        printf("\"file1\" and \"file2\" is same.\n");
        return 0;
    }

    if(stat(argv[2], &info) == -1)
    {
        printf("move file.\n");
        link(argv[1], argv[2]);
        remove(argv[1]);
        return 0;
    }
    if(S_ISDIR(info.st_mode))
    { // Directory
        printf("move to directory.\n");
        strcat(argv[2], "/");
        strcat(argv[2], argv[1]);
        link(argv[1], argv[2]);
        remove(argv[1]);
    }
    else
    { // Not directory
        printf("\"%s\" file already exist.\n", argv[2]);
    }

    return 0;

}


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t, char *, int);

int main()
{
    printpathto(get_inode("."));
    putchar('\n');
    return 0;
}

void printpathto(ino_t this_inode)
{
    int count = 0, i;
    ino_t my_inode;
    char its_name[BUFSIZ];
    char **node_name;
    
    node_name = (char**)malloc(sizeof(char*));
    
    while(get_inode("..") != this_inode)
    {
        count++;
        chdir("..");
        node_name = (char**)realloc(node_name, sizeof(char*));
        node_name[count-1] = (char*)malloc(BUFSIZ);
        inum_to_name(this_inode, its_name, BUFSIZ);
        strcpy(node_name[count-1], its_name);
        this_inode = get_inode(".");
    }

    for(i = count-1; i >= 0; i --)
        printf("/%s", node_name[i]);

    free(node_name);
}

void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen)
{
    DIR *dir_ptr;
    struct dirent *direntp;

    dir_ptr = opendir(".");
    if(dir_ptr == NULL) {
        perror(".");
        exit(1);
    }
    while((direntp = readdir(dir_ptr)) != NULL)
        if(direntp->d_ino == inode_to_find)
        {
            strncpy(namebuf, direntp->d_name, buflen);
            namebuf[buflen-1] = '\0';
            closedir(dir_ptr);
            return;
        }
    fprintf(stderr, "error looking for inum %d\n", inode_to_find);
    exit(1);
}

ino_t get_inode(char *fname)
{
    struct stat info;

    if(stat(fname, &info) == -1)
    {
        fprintf(stderr, "Cannot stat ");
        perror(fname);
        exit(1);
    }
    return info.st_ino;
}


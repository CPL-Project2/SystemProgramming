#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

void do_ls(char [], int flag_Recursive);
int dostat(char *, char *);
void show_file_info(char *, struct stat *);
void mode_to_letters(int , char[]);
char *uio_to_name(uid_t);
char *gio_to_name(gid_t);

int P_LENGTH = 1024;

void main(int ac, char *av[])
{
    int flag_Recursive = 0;

    if(ac > 1 && strcmp(av[1], "-R") == 0) flag_Recursive = 1;

    if(ac == 1) do_ls(".", flag_Recursive);
    else if(ac == 2 && flag_Recursive == 1) do_ls(".", flag_Recursive);
    else if(flag_Recursive == 0 && ac > 1){
        while(--ac) {
            printf("%s:\n", *++av);
            do_ls(*av, flag_Recursive);
        }
    }
    else printf("Wrong instruction\n");
}

void do_ls(char dirname[], int flag_Recursive)
{
    DIR *dir_ptr;
    struct dirent *direntp;
    int count_Directory = 0, i = 0;
    char name_Directory[100][P_LENGTH];
    char path[100];

//    printf("========== do_ls :: dirname = %s\n", dirname);
    if((dir_ptr = opendir(dirname)) == NULL) {
        fprintf(stderr, "ls2: cannot open %s\n", dirname);
    }
    else
    {
        while((direntp = readdir(dir_ptr)) != NULL)
        {
            strcpy(path, dirname);
            strcat(path, "/");
//            strcat(path, direntp->d_name);
//            strcpy(path, direntp->d_name);
            if(dostat(direntp->d_name, path))
            {
                count_Directory++;
                strcpy(name_Directory[i++], direntp->d_name);
            }
        }
        closedir(dir_ptr);
    }

    if(flag_Recursive)
    {
        for(i = 0; i < count_Directory; i ++) {
//            printf("========== dirname : %s\t\tname : %s>>>\n", dirname, name_Directory[i]);                    
            strcpy(path, dirname);
            strcat(path, "/");
            strcat(path, name_Directory[i]);
            printf("\n%s:\n", path);
            do_ls(path, 1);
//            printf("%s:\n", name_Directory[i]);
//            do_ls(name_Directory[i], 0);
        }
    }
}

int dostat(char *filename, char *path)
{
    struct stat info;
    char filepath[100];
    
    strcpy(filepath, path);
    strcat(filepath, filename);
    if(stat(filepath, &info) == -1) 
        perror(filename);
    else
        show_file_info(filename, &info);
    if(!strcmp(filename, ".")) return 0;
    if(!strcmp(filename, "..")) return 0;                
    return S_ISDIR(info.st_mode);
}

void show_file_info(char *filename, struct stat *info_p)
{
    char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
    void mode_to_letters();
    char modestr[11];

    mode_to_letters(info_p->st_mode, modestr);
    printf("%s",    modestr);
    printf("%4d ",   (int) info_p->st_nlink);
    printf("%-8s ",  uid_to_name(info_p->st_uid));
    printf("%-8s ",  gid_to_name(info_p->st_gid));
    printf("%8ld ",  (long)info_p->st_size);
    printf("%.12s ", 4+ctime(&info_p->st_mtime));
    printf("%s\n", filename);
}

void mode_to_letters(int mode, char str[])
{
    strcpy(str, "----------");
    
    if(S_ISDIR(mode))   str[0] = 'd';
    if(S_ISCHR(mode))   str[0] = 'c';
    if(S_ISBLK(mode))   str[0] = 'b';

    if(mode & S_IRUSR)  str[1] = 'r';
    if(mode & S_IWUSR)  str[2] = 'w';
    if(mode & S_IXUSR)  str[3] = 'x';

    if(mode & S_IRGRP)  str[4] = 'r';
    if(mode & S_IWGRP)  str[5] = 'w';
    if(mode & S_IXGRP)  str[6] = 'x';

    if(mode & S_IROTH)  str[7] = 'r';
    if(mode & S_IWOTH)  str[8] = 'w';
    if(mode & S_IXOTH)  str[9] = 'x';
}

char *uid_to_name(uid_t uid)
{
    struct passwd *getpwuid(), *pw_ptr;
    static char numstr[10];
    
    if((pw_ptr = getpwuid(uid)) == NULL) {
        sprintf(numstr, "%d", uid);
        return numstr;
    }
    else
        return pw_ptr->pw_name;
}

char *gid_to_name(gid_t gid)
{
    struct group *getgrgid(), *grp_ptr;
    static char numstr[10];
    
    if((grp_ptr = getgrgid(gid)) == NULL) {
        sprintf(numstr, "%d", gid);
        return numstr;
    }
    else
        return grp_ptr->gr_name;
}

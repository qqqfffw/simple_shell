#ifndef CELL_H
#define CELL_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>

#define Y   "\033[1;33m"
#define G   "\033[1;32m"
#define C   "\033[1;36m"
#define RED "\033[1;31m"
#define RST "\033[0m"

#define DEL "\n\t \v\f\r"
#define CELL_JR 0

typedef struct builtin {
    const char *builtin_name;
    int (*foo)(char **);
}t_builtin;


int cell_exit(char **args);
int  cell_env(char **args);

void Getcwd(char *, size_t);
void *Malloc(size_t);
void *Realloc(void*, size_t);

pid_t Fork(void);
void  Execvp(const char *file, char *const argv[]);
pid_t Wait(int *status);

void cell_launch(char **args);
void cell_launch_pipeline(char **left, char **right);
int  find_pipe(char **args);
void cell_exec(char **args);

#endif
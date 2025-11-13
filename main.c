#include "cell.h"

int status = 0;

//konvejeriai tik 2 komandos
int find_pipe(char **args) {
    for (int i = 0; args[i]; ++i){
        if (strcmp(args[i], "|") == 0){
            return i;
        }
    }
    return -1;
}

void cell_launch_pipeline(char **left, char **right){
    int fd[2];

    if(pipe(fd) == -1){
        status = 1;
        return;
    }

    pid_t pid1 = Fork();

    if (pid1 == CELL_JR) {
        if (dup2(fd[1], STDOUT_FILENO) == -1){
            exit(EXIT_FAILURE);
        }
        close(fd[0]);
        close(fd[1]);

        Execvp(left[0], left);
    }


    pid_t pid2 = Fork();
    if (pid2 == CELL_JR){
        if (dup2(fd[0], STDIN_FILENO) == -1){
            exit(EXIT_FAILURE);
        }
        close(fd[0]);
        close(fd[1]);

        Execvp(right[0], right);
    }

    close(fd[0]);
    close(fd[1]);

    Wait(&status);
    Wait(&status);
}


int cell_exit(char **args){
    (void)args;
    exit(EXIT_SUCCESS);
}

int cell_env(char **args){
    (void)args;

    extern char **environ;

    if(!environ){
        return 1;
    }

    for(int i = 0; environ[i]; i++){
        printf("%s\n", environ[i]);
    }

    return 0;
}

pid_t Fork(void){
    pid_t pid;
    pid = fork();

    if(pid < 0){
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    return pid;
}

void Execvp(const char *file, char *const argv[]){
    if(!file || !argv){
        fprintf(stderr, RED"Execvp failed\n"RST);
        exit(EXIT_FAILURE);
    }
    if(execvp(file, argv) == -1){
        perror(RED"CELL_JR failed"RST);
        exit(EXIT_FAILURE);
    }
}

pid_t Wait(int *status){
    if (!status) {
        fprintf(stderr, RED "Wait failed no args\n" RST);
        return -1;
    }

    pid_t result = wait(status);
    if (result == -1){
        perror(RED "Wait failed" RST);
        return -1;
    }

    if (WIFEXITED(*status)){
        *status = WEXITSTATUS(*status);
    }

    return result;
}

void cell_launch(char **args){
    if(Fork() == CELL_JR){
        Execvp(args[0], args);
    }else
        Wait(&status);
}

t_builtin g_builtin[] = {
    {.builtin_name="exit", .foo=cell_exit},
    {.builtin_name=NULL, .foo=NULL}
};



void cell_exec(char **args){
    if (!args || !args[0]){
        return;
    }

    int pipe_pos = find_pipe(args);
    if (pipe_pos != -1) {
        //padaliname i kaire arba desine
        args[pipe_pos] = NULL;
        char **left  = args;
        char **right = &args[pipe_pos + 1];

        if (!right[0]) {
            fprintf(stderr, RED "Klaida | parasytas bet nieko po juo\n" RST);
            return;
        }

        cell_launch_pipeline(left, right);
        return;
    }

    //exit komanda arba built in komandos
    int i = 0;
    const char *current;

    while ((current = g_builtin[i].builtin_name)) {
        if (!strcmp(current, args[0])) {
            status = g_builtin[i].foo(args);
            return;
        }
        ++i;
    }

    cell_launch(args);
}

//iskirsto i tokenus pvz ls -la iskirstys i ls ir -la naudodjant space " "
char **cell_split_line(char *line){
    char **tokens;
    int position;
    size_t bufsize;

    bufsize = BUFSIZ;
    tokens = Malloc(BUFSIZ * sizeof *tokens);
    position = 0;

    for(char *token = strtok(line, DEL); token; token = strtok(NULL, DEL)){
        tokens[position++] = token;

        if(position >= bufsize){
            bufsize *= 2;
            tokens = Realloc(tokens, bufsize * sizeof(*tokens));
        }
    }

    tokens[position] = NULL;
    return tokens;
}




char *cell_read_line(void){
    char *buf;
    size_t bufsize;
    char cwd[BUFSIZ];

    buf = NULL;
    

    Getcwd(cwd, sizeof(cwd));
    printf(Y"%s"RST RED" $>"RST, cwd);



    if(getline(&buf, &bufsize, stdin) == -1){

        free(buf);
        buf = NULL; //del eof isvalom buf
        if(feof(stdin)){
            printf(RED"\nEOF\n"RST);
        }else
            printf(RED"\nGetline failed\n"RST);

    }

    return buf;
}

void Getcwd(char *buf, size_t size){
    if(NULL == getcwd(buf, size))
        perror(RED"getcwd failed"RST);
}


int main(){
    //REPL
    char *line;
    char **args;

    while((line = cell_read_line())){
        //get line

        //printf("%s\n", line);

        //tokenization
        args = cell_split_line(line);

        // for(int i = 0; args[i]; ++i){
        //     printf("%s\n", args[i]);
        // }

        //exec
        cell_exec(args);

        //free
        free(line);
        free(args);
    }

    return EXIT_SUCCESS;
}
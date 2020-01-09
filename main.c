#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#define MAXARGUMENTS 128

struct command {
    int argc;//number of arguments
    char *argv[MAXARGUMENTS];
};

int MAXLINE = 1024;//max line size
char name[] = "myShell> ";
void takeAndAddHistory(char* input);
void execute(char *commands);
void parseThenRun(char *commands);
void runAll(int numberOfCommands,struct command** allCommands);
void runPiped (int in, int out, struct command *cmd);
void parse(char *cmdline, struct command *cmd);
int main(int argc, char **argv)
{
    char input[MAXLINE];//BUFFER FOR INPUT
    while(1){//menu
    takeAndAddHistory(input);//take input and add history
    if((input[0]=='c')&&(input[1]=='d')&&(input[3]!=' ')){
                chdir(&input[3]);
            }
    else{
    execute(input);//execute that input
    }
    }
}

void takeAndAddHistory(char* input){
    char* buf;
    buf = readline("myShell> ");
    if(strlen(buf) != 0){
        add_history(buf);
        strcpy(input,buf);
    }
}
void execute(char *commands){
    //mainprocess creates child
    pid_t childPid;
    if((childPid = fork())<0)//if fork fails return{
    {
    printf("Fork error");
    return;
    }
    else if(childPid == 0){//if we are in the child
        parseThenRun(commands);//runn all commands in it
    }
    else{//we are parent
    wait(&childPid);//wait for child to finish all commands then return menu
    return;
    }
}
void parseThenRun(char *commands){
    char **tokenizedLines;//list for each tokenized chars
    tokenizedLines = malloc(1*sizeof(char*));//initialize with size of 1
    int i = 0;//for tokenizing
    struct command** allCommands;//each tokenized string will be parsed into one command
    int j = 0;//for appending to command list
    char *token;
    //start tokenizing
    token = strtok(commands, "|");
    while(token != NULL){
        tokenizedLines[i++] = token;//copy it to the array
        tokenizedLines = realloc(tokenizedLines,(i+1)*sizeof(char*));//dynamic memory
        token = strtok(NULL, "|");
    }
    allCommands = malloc(i*sizeof(struct command*));//i = number of piped commands
    for( j = 0; j<i; j++){
        allCommands[j] = malloc(1*sizeof(struct command));//for each command create memory
        parse(tokenizedLines[j],allCommands[j]);//parse corresponding command and append it
    }
    runAll(i,allCommands);//everything is ready so we can start i = number of commands
    //free memory
    for(j = 0; j<i ; j++){
        free(tokenizedLines[j]);
        free(allCommands[j]);
    }
    free(tokenizedLines);
    free(allCommands);
}

void runAll(int numberOfCommands,struct command** allCommands){
  int i = 0;//iterator for pipe
  int in, fd [2];//file descriptors,
  in = 0;//First command should get 0th output
   for (i = 0; i < numberOfCommands - 1; ++i)
    {
      pipe (fd);//pipe all descriptors
      runPiped (in, fd [1], allCommands[i]);//run each with in= input, fd = output
      close (fd [1]);//close it so fd[0] is fd[1] now
      in = fd [0];//next process input is output of
    }
    //last step to print all results,
  if (in != 0)
    dup2 (in, 0);//set last processes input to piped output
        if(execvp (allCommands[i]->argv [0], (char * const *)allCommands[i]->argv)<0){
            printf("%s : Command not found\n",allCommands[i]->argv [0]);

            }

}
void runPiped (int in, int out, struct command *cmd)
{
  pid_t pid;
  if ((pid = fork ()) == 0){//child will run
      if (in != 0)//
        {
          dup2 (in, 0);//set input to current input
          close (in);
        }
      if (out != 1)
        {//
          dup2 (out, 1);//set output to current childs output
          close (out);
        }
      execvp (cmd->argv [0], (char * const *)cmd->argv);//run command
    }
}
void parse(char *cmdline, struct command *cmd){
    int i = 0;
    char *token;
    token = strtok(cmdline," ");
    while(token!=NULL){
        cmd->argv[i]= token;
        token = strtok(NULL," ");
        i++;
    }
    cmd->argv[i] = NULL;
    cmd->argc = i;
    return;
}

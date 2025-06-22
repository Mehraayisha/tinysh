#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#define MAX_LINE 1024
#define MAX_ARGS 64
void prompt()
{
  printf("tinysh>>");
  fflush(stdout);
}
void execute_cmd(char *input)
{
  char *args[MAX_ARGS];
  int i=0;
  char *token=strtok(input," ");
  while(token != NULL && i<MAX_ARGS)
  {
   args[i++]=token;
   token=strtok(NULL," ");
  }
  args[i]=NULL;

  if(args[0]==NULL)
  return;

  if(strcmp(args[0],"exit")==0)
  exit(0);

  pid_t pid=fork();
  if(pid == 0)
  {
   execvp(args[0],args);
   perror("command failed");
   exit(1);
  }
  else if(pid>0)
  {
   wait(NULL);
  }
  else
  {
   perror("fork failed");
  }

}
int main()
{
  char input[MAX_LINE];
  while(1)
  {
    prompt();
    if(fgets(input,sizeof(input),stdin)==NULL)
    {
     printf("\n");
     break;
    }
    input[strcspn(input,"\n")]=0;
    execute_cmd(input);
  }
  return 0;
}


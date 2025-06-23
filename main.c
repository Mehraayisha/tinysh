#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#define MAX_LINE 1024
#define MAX_ARGS 64
void prompt()
{
  printf("tinysh>> ");
  fflush(stdout);
}
void change_dir(char **args)
{
   char *target_dir=args[1];
   if(target_dir == NULL)
   {
    printf("enter a target directory\n");
    return;
   }
   else if(strcmp(target_dir,"-")==0)
  {
   //getenv get the value of OLDPWD
   target_dir=getenv("OLDPWD");
     if(target_dir==NULL)
     {
       fprintf(stderr,"cd:No previous directory\n");
       return;
     }
     printf("%s\n",target_dir);
  }
  //chdir return 0 if success else -1
  if(chdir(target_dir)!= 0)
   {
    perror("cd");
    return;
   }
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
  
  if(strcmp(args[0],"cd")==0)
  {
   change_dir(args);
   return;
   
  }
  
  pid_t pid=fork();
  if(pid == 0)
  {
   execvp(args[0],args);
   perror("command failed\n");
   exit(1);
  }
  else if(pid>0)
  {
   wait(NULL);
  }
  else
  {
   perror("fork failed\n");
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


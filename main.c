#include "shell.h"
#define MAX_LINE 1024
#define MAX_ARGS 64

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
int execute_cmd(char *input)
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
  {
    return 0;
  }

  if(strcmp(args[0],"exit")==0)
  {
   return 1;
  }
  
  if(strcmp(args[0],"cd")==0)
  {
    change_dir(args);
    return 0;
   
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
  return 0;

}



int main()
 {
  read_history(".tinysh_history");
  
  while(1)
  {
    char *input_line=readline("tiny>> ");
    if(input_line==NULL)
    {
     printf("\n");
     break;
    }
    //if input_line isnt empty
    if(*input_line)
    {
      add_history(input_line);//save to history

      if(execute_cmd(input_line))//if user type "exit"
      {
        free(input_line);
        break;
      }
    }
    free(input_line);
    
  }
  
 write_history(".tinysh_history");  

  return 0;
}


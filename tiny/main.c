#include "shell.h"
#define MAX_LINE 1024
#define MAX_ARGS 64
#define MAX_CMD 10
//remove space from begining and ending of string
void trim(char *str) {
    
    int start = 0;
    while (str[start] == ' ') start++;
    
    if (start > 0) {
        //move the string to left to remove the space
        memmove(str, str + start, strlen(str + start) + 1); 
    }

    
    int end = strlen(str) - 1;
    while (end >= 0 && (str[end] == ' ' || str[end] == '\n')) {
        str[end] = '\0';
        end--;
    }
}

void split_pipe(char *input,char **cmd)
{
  
  int num=0;
  cmd[0]=strtok(input,"|");
  while(cmd[num]!=NULL && num<MAX_CMD-1)
  {
    trim(cmd[num]);
    cmd[++num]=strtok(NULL,"|");


  }
  cmd[num]=NULL;
}
void split_args(char *cmd,char **args)
{
  int i=0;
  char *token=strtok(cmd," ");
  while(token != NULL && i<MAX_ARGS)
  {
   args[i++]=token;
   token=strtok(NULL," ");
  }
  args[i]=NULL;

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
/*So first, the parent creates a pipe (pipe1), then forks the first child (e.g., for ls).
That child uses dup2(pipe1[1], 1) to redirect its output (stdout) into the pipe, so instead of printing to the screen, it writes to pipe1[1].
Then it executes ls, so ls writes its output into the pipe.

The parent closes pipe1[1] (write end) and keeps pipe1[0] (read end) in a variable called in_fd — this is used as input for the next command.

Next, the parent creates another pipe (pipe2) and forks the second child (e.g., for grep).
This child uses dup2(in_fd, 0) to read its input from the previous pipe’s read end (pipe1[0]), and
uses dup2(pipe2[1], 1) to send its output to the next pipe (pipe2[1]).
Then it runs grep, so grep reads from ls and writes to the next pipe.

Again, the parent closes old FDs and updates in_fd = pipe2[0].

Finally, the parent forks the third child (e.g., for wc -l).
This child uses dup2(in_fd, 0) to read input from the second pipe (pipe2[0]) and writes normally to the screen (no redirection needed if it's the last command)*/
int execute_cmd(char *input)
{
  char *cmd[MAX_CMD];
  char *args[MAX_ARGS];
  split_pipe(input,cmd);
  

  if(cmd[0]==NULL)
  {
    return 0;
  }

  if(cmd[1]==NULL)
  {
      split_args(cmd[0],args);

        if(args[0]==NULL)
        {
          return 0;
        }
        if(strcmp(args[0],"help")==0)
        {
          FILE *fp=fopen("tinysh.man","r");
          if(fp==NULL)
          {
            perror("Unable to load help manual");
            return 0;
          }
          char line[256];
          while(fgets(line,sizeof(line),fp))
          {
            printf("%s",line);
          }
          fclose(fp);
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
        //chatbot
        if(strcmp(args[0],"/ask")==0)
        {
          if(args[1]==NULL)
          {
            printf("Usage: /ask <your question>\n");
            return 0;
          }
          char question[1024]="";
          for(int i=1;args[i]!=NULL;i++)
          {
            strcat(question,args[i]);
            strcat(question," ");
          }
          
            char *api_key = getenv("GROQ_API_KEY");
    if (api_key == NULL) {
        fprintf(stderr, "Error: GROQ_API_KEY not set. Use export GROQ_API_KEY=your_key\n");
        return 0;
    }

    // Format curl command to call Groq
    //curl is  command-line tool for HTTP request (-s means silent mode )this sends a post request
    
    char command[4096];
    snprintf(command, sizeof(command),
        "curl -s https://api.groq.com/openai/v1/chat/completions "
        "-H \"Authorization: Bearer %s\" "
        "-H \"Content-Type: application/json\" "
        "-d '{\"model\": \"llama3-8b-8192\", \"messages\": [{\"role\": \"user\", \"content\": \"%s\"}]}' "
        "| jq -r '.choices[0].message.content'",
        api_key, question);

    // Execute the curl command
    system(command);
    return 0;
}
else if (strcmp(args[0],"/weather")==0)
{
  system("python3 ../plugins/weather.py");
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
 //if pipe is used
 else{
     
     int in_fd=0;//initialiy input from keyboards
     int pipefd[2];
     pid_t pid;
     for(int i=0;cmd[i]!=NULL;i++)
     {
      split_args(cmd[i],args);
      if(args[0]==NULL) continue;
      //create pipe if more than one command exist
      if(cmd[i+1]!=NULL)
      {
        if(pipe(pipefd)<0)
         {
          perror("pipe cant be created");
          return 0;
         }

      }
      pid=fork();
      if(pid==0)
      {
        //make the cmd read from pipe if its not the first cmd
         if(in_fd!=0)
         {
          //redirects stdin to read from in_fd     0 -> stdin
          dup2(in_fd,0);
          close(in_fd);
         }
         //make the cmd write to pipe if its not the last cmd
        if(cmd[i+1]!=NULL)
        {
          //Makes stdout go into the pipe 1 -> stdout
          dup2(pipefd[1],1);
          close(pipefd[1]);
          close(pipefd[0]);
        }
        execvp(args[0],args);
        perror("execvp failed");
        exit(1);
       

      }
      else if (pid<0)
      {
        perror("fork failed");
        return 0;
      }
      if(in_fd!=0)
      close(in_fd);
      //close the write end
      if(cmd[i+1]!=NULL)
      {
        close(pipefd[1]);
        in_fd=pipefd[0];
      }
      wait(NULL);

     }
  
  return 0;

}
}


int main()
 {
  read_history(".tinysh_history");
  
  while(1)
  {
    char prompt[100];
    snprintf(prompt, sizeof(prompt),
    "\033[38;2;204;153;204m tiny>> \033[0m");
    char *input_line = readline(prompt);

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


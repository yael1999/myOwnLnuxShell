// solved: 1, 2, 3, 4, 5

#include <sys/stat.h>



#include <sys/wait.h>



#include <fcntl.h>



#include "stdio.h"



#include "errno.h"



#include "stdlib.h"



#include "unistd.h"



#include <string.h>



void my_handler(int sig)



{



    printf("\nControl-C Ignored\n");

}



int main()



{



    char command[1024];



    char command2[1024];



    char command3[1024];



    char *token;



    char *outfile;



    int i, fd, amper, redirect, retid, status;



    char *argv[10];



    //char *prompt = 0;



    int redirectnew = 0;



    char prompt[20];



    int repeat = 0; // for !!



    signal(SIGINT, my_handler); //if user presses ctrl c ,que 5



    strcpy(prompt, "hello:");



    while (1)



    {



        //  printf("hello: ");



        if (repeat)



        {



            strcpy(command, command2);



            repeat = 0;

        }



        else



        {



            printf("%s", prompt);



            fgets(command, 1024, stdin);



            strcpy(command3, command);

        }



        command[strlen(command) - 1] = '\0';



        //printf("\"%s\"\n", command);



        //exit(0);



        /* parse command line */



        argv[0] = NULL;



        i = 0;



        token = strtok(command, " ");



        while (token != NULL)



        {



            argv[i] = token;



            //printf ("%s\n",token);



            token = strtok(NULL, " ");



            i++;

        }



        argv[i] = NULL;



        /* Does command line end with & */



        if (argv[0] && !strcmp(argv[i - 1], "&"))



        {



            amper = 1;



            argv[i - 1] = NULL;

        }



        else



            amper = 0;



        //check if to quit



        if (strcmp(argv[0], "quit") == 0) //que 4



        {



            exit(0);

        }



        if (argv[0] && !strcmp(argv[0], "!!")) //if first string is  !! , que 3



        {



            repeat = 1;



            continue;

        }



        strcpy(command2, command3);



        if (argv[0] && argv[1] && !strcmp(argv[i - 2], ">"))



        {



            redirect = 1;



            argv[i - 2] = NULL;



            outfile = argv[i - 1];



            // f = fork();

        }



        else if (argv[0] && argv[1] && !strcmp(argv[i - 2], ">>")) // <---



        {



            redirect = 2;



            argv[i - 2] = NULL;



            outfile = argv[i - 1];

        }



        else



            redirect = 0;



        /* for commands not part of the shell command language */



        if (argv[0] && !strcmp(argv[0], "prompt"))



        {



            if (!strcmp(argv[1], "="))



                strcpy(prompt, argv[2]);



            continue;

        }



        if (fork() == 0)



        {



            /* redirection of IO ? */



            if (redirect == 1)



            {



                fd = creat(outfile, 0660);



                close(STDOUT_FILENO); //only for child



                dup(fd); //only for child



                close(fd);



                /* stdout is now redirected */

            }



            if (redirect == 2) //for 2 >>



            {

                fd = open(argv[i - 1], O_WRONLY | O_APPEND | O_CREAT, 0777);



                close(STDOUT_FILENO); //only for child



                dup(fd); //only for child



                close(fd);



                /* stdout is now redirected */

            }



            execvp(argv[0], argv);

        }



        /* parent continues here */



        if (amper == 0)



            retid = wait(&status);

    }

}


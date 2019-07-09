/******************************************
* Student name: Atara Razin
* Student: 327348934
* Course Exercise Group: 8923106
* Exercise name: Exercise 2
******************************************/
#include <termio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
void exitElegantly(int sig);
pid_t firstSecond;
pid_t secondSon;

/***********************************************************************
 * function name: main
 * input: arguments to main. gets the amout of time we want the program to run
 * output: returns 0 if it gets to the end
 * operation:forks and runs both executable files and sends the output
 * of one as the input of the other
*************************************************************************/

int main(int argc, char *argv[]){
    int runtTimeOfProgram = atoi(argv[1]);
    int stat, waited;

    struct sigaction usr_action;
    sigset_t block_mask;

    sigfillset (&block_mask);
    usr_action.sa_handler = exitElegantly;
    usr_action.sa_mask = block_mask;
    usr_action.sa_flags = 0;
    sigaction (SIGALRM, &usr_action, NULL);

    alarm(runtTimeOfProgram);

    int fd[2];
    pipe(fd);

    char inputFile[] = "./ex2_inp.out";
    pid_t firstSon = fork();
    if(firstSon < 0){
        perror("error forking");
        exit(-1);
    }
    else if (firstSon == 0) {   //son
        if(dup2(fd[0],0)<0){
            perror("error duping!");
            exit(-1);
        }

        if(close(fd[0])<0){
            perror("error closing");
            exit(-1);
        }
        if(close(fd[1])<0){
            perror("error closing");
            exit(-1);
        }

        char *args2[] = {inputFile, 0};
        int ret_code = execvp(inputFile, args2);
        if (ret_code == -1) {
            perror("exec failed! ");
            return -1;
        }
    } else {  //father
        sleep(1);
        char updateFile[] = "./ex2_upd.out";
        pid_t secondSon = fork();
        if(secondSon<0){
            perror("error forking");
            exit(-1);
        }
        else if(secondSon == 0)
        {//son
            char arg1[10000] = {0};
            sprintf(arg1, "%d", firstSon);
            char *args3[] = {updateFile, arg1,0};

            if(dup2(fd[1],1)<0){
                perror("error!");
                exit(-1);
            }

            if(close(fd[0])<0){
                perror("error closing");
                exit(-1);
            }
            if(close(fd[1])<0){
                perror("error");
                exit(-1);
            }

            int ret_code = execvp(updateFile, args3);
            if (ret_code == -1) {
                perror("exec failed second son! ");
                return -1;
            }
        }
        else {//father
            int a;
            waitpid(firstSon, &a, 0);
            waitpid(secondSon, &a, 0);
            if(close(fd[0])<0){
                perror("eroor closing");
                exit(-1);
            }
            if(close(fd[1])<0){
                perror("eroor closing");
                exit(-1);
            }
        }


    }
}

/***********************************************************************
 * function name: exitElegantly
 * input: int sig
 * output: exits with 1
 * operation:kills both sons and then exits
*************************************************************************/

void exitElegantly(int sig){
    if(kill(firstSecond,SIGINT)<0){
        perror("trouble killing\n");
        exit(-1);
    }

    if (kill(secondSon,SIGINT)<0){
        perror("cant killing\n");
        exit(-1);
    }
    exit(1);
}

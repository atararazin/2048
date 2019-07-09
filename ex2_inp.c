/******************************************
* Student name: Atara Razin
* Student: 327348934
* Course Exercise Group: 8923106
* Exercise name: Exercise 2
******************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

#define SIZE 16
void sigIntHandler(int sig);
void signal_hand(int sig);


/***********************************************************************
 * function name: main
 * input: none
 * output: returns 0 if it gets to the end
 * operation: makes the sigaction and defines the masks for sigint and sigusr1.
 * pauses the program until it gets a sig int.
*************************************************************************/
int main()
{
    struct sigaction usr_action;
    sigset_t block_mask;
    struct sigaction usr_action1;
    sigset_t block_mask1;

    //for sigusr1
    sigfillset (&block_mask);
    sigdelset(&block_mask,SIGINT);
    usr_action.sa_handler = signal_hand;
    usr_action.sa_mask = block_mask;
    usr_action.sa_flags = 0;

    //for sigint
    sigfillset(&block_mask1);
    usr_action1.sa_handler = sigIntHandler;
    usr_action1.sa_mask = block_mask1;
    usr_action1.sa_flags = 0;

    sigaction (SIGUSR1, &usr_action, NULL);
    sigaction(SIGINT, &usr_action1, NULL);

    int didntGetSigInt = 1;
    while (didntGetSigInt == 1){
        pause();
    }
    return 0;
}


/***********************************************************************
 * function name: signal_hand
 * input: int sig
 * output: void
 * operation: handles a sigusr1. reads in the numbers and prints them in a
 * user format.
*************************************************************************/
void signal_hand(int sig){
    int endOfGameFlag = 0;
    int numbers[SIZE];
    int *ptr = numbers;
    char comma;

    //reads in numbers
    int i;
    for(i=0;i<SIZE; i++) {
        //if last, dont read in comma
        if(i == SIZE - 1){
            scanf("%i", ptr);
            numbers[i] = *ptr;
            break;
        }
        scanf("%i%c", ptr, &comma);
        numbers[i] = *ptr;
        //checks if the game is over
        if(numbers[i] == -1 || numbers[i] == -2 ){
            endOfGameFlag = 1;
            break;
        }
        ptr++;
    }

    //print if the game is over
    if(endOfGameFlag == 1){
        if(numbers[0] == -1){

            printf("Congratulations!\n");
        }
        else{
            printf("Game Over\n");
        }
        exit(1);
    }

    //print it in the format
    for (i = 0; i < SIZE; i+=4) {
        if(numbers[i] != 0){
            printf("| %04i |",numbers[i]);
        }
        else{
            printf("|      |");
        }

        if(numbers[i+1] != 0){
            printf(" %04i |",numbers[i+1]);
        }
        else{
            printf("      |");
        }

        if(numbers[i+2] != 0){
            printf(" %04i |",numbers[i+2]);
        }
        else{
            printf("      |");
        }


        if(numbers[i+3] != 0){
            printf(" %04i |\n",numbers[i+3]);
        }
        else{
            printf("      |\n");
        }
    }
    printf("\n");
}

/***********************************************************************
 * function name: sigIntHandler
 * input: int sig
 * output: void
 * operation: handles a sigint. prints bye bye and exits elegantly
*************************************************************************/
void sigIntHandler(int sig){
    printf("BYE BYE\n");
    exit(1);
}

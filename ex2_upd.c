/******************************************
* Student name: Atara Razin
* Student: 327348934
* Course Exercise Group: 8923106
* Exercise name: Exercise 2
******************************************/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#define SIZE 16

unsigned int timeTillAddNewNumber;
int numbers[SIZE];
pid_t pidOfProcess1;
char numberInStringForm[40000];

void makeNewBoard();
void addANewTwo();
void moveLeft();
void moveRight();
void moveUp();
void moveDown();
void consolidateToLeft(int start, int end);
void combineSameNumbersLeft(int start, int end);
void consolidateToRight(int start, int end);
void combineSameNumbersRight(int start, int end);
void consolidateToUp(int start, int end);
void combineSameNumbersUp(int start, int end);
void consolidateToDown(int start, int end);
void combineSameNumbersDown(int start, int end);
void exitElegantly(int sig);
int sendAndWriteResults();
void fromArrayToString();
int didLoose();
int didWin();
void sendWonOrLost(int i);

/***********************************************************************
 * function name: main
 * input: the pid of the other process the one that prints
 * output: returns 0 if it gets to the end
 * operation:defines both sigactions and mask. sets the alarm and runs a
 * while loop that either moves or makes a new board. checks if it won or
 * lost meaning if the game ended.
*************************************************************************/
int main(int argc, char* argv[]){
    //for sigalarm
    struct sigaction usr_action;
    sigset_t block_mask;

    sigfillset (&block_mask);
    //lets sigint happen
    sigdelset(&block_mask,SIGINT);
    usr_action.sa_handler = addANewTwo;
    usr_action.sa_mask = block_mask;
    usr_action.sa_flags = 0;
    sigaction (SIGALRM, &usr_action, NULL);

    ///for sigint
    struct sigaction usr_action1;
    sigset_t block_mask1;

    sigemptyset(&block_mask1);
    usr_action1.sa_handler = exitElegantly;
    usr_action1.sa_mask = block_mask1;
    usr_action1.sa_flags = 0;
    sigaction (SIGINT, &usr_action1, NULL);

    int loose= 0;
    int win = 0;
    pidOfProcess1 = atoi(argv[1]);
    char inputChar;
    makeNewBoard();

    while(!loose && !win){
        alarm(timeTillAddNewNumber);

        //makes getchar work witout enter
        system("stty cbreak -echo");
        inputChar = getchar();
        system("stty cooked echo");

        switch(inputChar){
            case 'A':
                moveLeft();
                sendAndWriteResults();
                break;
            case 'D':
                moveRight();
                sendAndWriteResults();
                break;
            case 'X':
                moveDown();
                sendAndWriteResults();
                break;
            case 'W':
                moveUp();
                sendAndWriteResults();
                break;
            case 'S':
                makeNewBoard();
                break;
            default:
                break;
        }


        if(didLoose() == 1){
            loose = 1;
        }
        if(didWin() == 1){
            win =1;
        }
    }

    /*sends thesee negatives instead of numbers to
     notify theother that the game ended*/
    if(loose == 1){
        sendWonOrLost(-2);
    }
    if(win == 1){
        sendWonOrLost(-1);
        exit(1);
    }
    return 0;
}

/***********************************************************************
 * function name: makeNewBoard
 * input: none
 * output: none
 * operation:defines all the numbers as 0. then gets the first two and then
 * the second. amkes sure theyre not the same numbers. calls function
 * to send and write results
*************************************************************************/
void makeNewBoard(){
    int firstTwo, secondTwo;
    int i;
    for(i =0; i<SIZE;i++){
        numbers[i] = 0;
    }

    srand(time(NULL));
    firstTwo = rand() % SIZE;
    while((secondTwo = rand() %SIZE) == firstTwo);
    numbers[firstTwo] = 2;
    numbers[secondTwo] = 2;

    sendAndWriteResults();
}

/***********************************************************************
 * function name: moveLeft
 * input: none
 * output: none
 * operation:algorithm:
 * move all to the left as much as they
 * can combine all that can to the left spot
 * move all to the left as much as they can
*************************************************************************/
void moveLeft() {
    int i;
    for (i = 0; i < SIZE; i += 4) {
        consolidateToLeft(i, i + 3);
        combineSameNumbersLeft(i, i + 3);
        consolidateToLeft(i, i + 3);
    }
}

/***********************************************************************
 * function name: consolidateToLeft
 * input: int start, int end
 * output: none
 * operation: consoladates all leftwise
*************************************************************************/
    void consolidateToLeft(int start, int end) {

        int emptySpace = start;
        int i;
        for (i = start; i <= end; i++) {
            if (numbers[i] != 0) {
                numbers[emptySpace] = numbers[i];
                emptySpace++;
            }
        }

        for (i = emptySpace; i <= end; i++) {
                numbers[i] = 0;
            }
    }


/***********************************************************************
 * function name: combineSameNumbersLeft
 * input: int start, int end
 * output: none
 * operation: if two numbers are the same adds them
*************************************************************************/
    void combineSameNumbersLeft(int start, int end) {

        int i = start + 1;
        while (numbers[i] != 0 && i<=end) {
            if (numbers[i] == numbers[i - 1]) {
                numbers[i - 1] += numbers[i];
                numbers[i] = 0;
            }
            i++;
        }
    }


/***********************************************************************
 * function name: moveRight
 * input: none
 * output: none
 * operation:algorithm:
 * move all to the rigth as much as they
 * can combine all that can to the right spot
 * move all to the right as much as they can
*************************************************************************/
void moveRight() {
    int i;
        for (i = 0; i < SIZE; i += 4) {
            consolidateToRight(i, i + 3);
            combineSameNumbersRight(i, i + 3);
            consolidateToRight(i, i + 3);
        }
}

/***********************************************************************
 * function name: consolidateToRight
 * input: int start, int end
 * output: none
 * operation: consoladates all rightwise
*************************************************************************/
void consolidateToRight(int start, int end) {
    int emptySpace = end;
    int i;
    for (i = end; i >= start; i--) {
        if (numbers[i] != 0) {
            numbers[emptySpace] = numbers[i];
            emptySpace--;
        }
    }

    for (i = emptySpace; i >= start; i--) {
        numbers[i] = 0;
    }
}


/***********************************************************************
 * function name: combineSameNumbersRight
 * input: int start, int end
 * output: none
 * operation: if two numbers are the same adds them
*************************************************************************/
void combineSameNumbersRight(int start, int end) {
    int i = end -1;
    while (numbers[i] != 0 && i>=start) {
        if (numbers[i] == numbers[i + 1]) {
            numbers[i + 1] += numbers[i];
            numbers[i] = 0;
        }
        i--;
    }
}

/***********************************************************************
 * function name: moveUp
 * input: none
 * output: none
 * operation:algorithm:
 * move all to the up as much as they
 * can combine all that can to the up spot
 * move all up as much as they can
*************************************************************************/
void moveUp() {
    int i;
    for(i = 0; i<4; i++){
        consolidateToUp(i,i+12);
        combineSameNumbersUp(i,i+12);
        consolidateToUp(i,i+12);
    }
}

/***********************************************************************
 * function name: consolidateToUp
 * input: int start, int end
 * output: none
 * operation: consoladates all upwise
*************************************************************************/
void consolidateToUp(int start, int end) {
    int emptySpace = start;
    int i;
    for (i = start; i <= end; i+=4) {
        if (numbers[i] != 0) {
            numbers[emptySpace] = numbers[i];
            emptySpace+=4;
        }
    }

    for (i = emptySpace; i <= end; i+=4) {
        numbers[i] = 0;
    }

}


/***********************************************************************
 * function name: combineSameNumbersUp
 * input: int start, int end
 * output: none
 * operation: if two numbers are the same adds them
*************************************************************************/
void combineSameNumbersUp(int start, int end) {
    int i = start + 4;
    while (numbers[i] != 0 && i<=end) {
        if (numbers[i] == numbers[i - 4]) {

            numbers[i - 4] += numbers[i];
            numbers[i] = 0;

        }
        i+=4;
    }

}

/***********************************************************************
 * function name: moveDown
 * input: none
 * output: none
 * operation:algorithm:
 * move all down as much as they
 * can combine all that can down spot
 * move all down as much as they can
*************************************************************************/
void moveDown() {
    int i;
    for(i = 0; i<4; i++){
        consolidateToDown(i,i+12);
        combineSameNumbersDown(i,i+12);
        consolidateToDown(i,i+12);
    }
}

/***********************************************************************
 * function name: consolidateToDown
 * input: int start, int end
 * output: none
 * operation: consoladates all rightwise
*************************************************************************/
void consolidateToDown(int start, int end) {

    int emptySpace = end;
    int i;
    for (i = end; i >= start; i-=4) {
        if (numbers[i] != 0) {
            numbers[emptySpace] = numbers[i];
            emptySpace-=4;
        }
    }

    for (i = emptySpace; i >= start; i-=4) {
        numbers[i] = 0;
    }
}


/***********************************************************************
 * function name: combineSameNumberDown
 * input: int start, int end
 * output: none
 * operation: if two numbers are the same adds them
*************************************************************************/
void combineSameNumbersDown(int start, int end) {
    int i = end - 4;
    while (numbers[i] != 0 && i>=start) {
        if (numbers[i] == numbers[i + 4]) {

            numbers[i + 4] += numbers[i];
            numbers[i] = 0;

        }
        i-=4;
    }

}

/***********************************************************************
 * function name: exitElegantly
 * input: none
 * output: none
 * operation: exits with 1
*************************************************************************/
void exitElegantly(int sig){
    exit(1);
}

/***********************************************************************
 * function name: addANewTwo
 * input: int sig
 * output: none
 * operation: checks where there are open slots and amonst them it puts in
 * a two. then it makes a new timetilladdnewnumber. calls and send results
*************************************************************************/
void addANewTwo(int sig){
    int temp[SIZE]={0};
    int numOfOpenSlots=0;
    int i;
    //checks open slots
    for(i =0; i<SIZE;i++){
        if(numbers[i] == 0){
            temp[numOfOpenSlots] = i;
            numOfOpenSlots++;
        }
    }

    srand(time(NULL));
    int addATwo = rand() % numOfOpenSlots;
    numbers[temp[addATwo]] = 2;
    timeTillAddNewNumber = (rand() % 5) + 1;

    sendAndWriteResults();
}

/***********************************************************************
 * function name: sendAndWriteResults
 * input: none
 * output: an int
 * operation: makes a new waiting time. then writes to stdout. sends with
 * kill a sigusr to the other process.
*************************************************************************/
int sendAndWriteResults(){
    //make a new waiting time
    srand ( time(NULL) );
    timeTillAddNewNumber = (rand() % 5) + 1;

    //write to STDOUT
    fromArrayToString();
    write(1,numberInStringForm, strlen(numberInStringForm));

    //send signal to process 1
    if(kill (pidOfProcess1,SIGUSR1)<0){
        perror("error sending write and restuls!\n");
        exit(-1);
    }
}

/***********************************************************************
 * function name: fromArrayToString
 * input: none
 * output: an int
 * operation: converts from an array of ints to string format. with sprintf
*************************************************************************/
void fromArrayToString(){
    char string[100000] = {0};

    memset(numberInStringForm,0,40000);
    sprintf(string,"%d%c",numbers[0],',');
    strcpy(numberInStringForm,string);
    int i;
    for(i =1; i<SIZE-1;i++) {
        memset(string,0,40000);
        sprintf(string,"%d%c",numbers[i],',');
        strcat(numberInStringForm,string);
    }
    memset(string,0,40000);
    sprintf(string,"%d\n",numbers[SIZE -1]);
    strcat(numberInStringForm,string);
}

/***********************************************************************
 * function name: didLoose
 * input: none
 * output: an int
 * operation: checks if theres an open slot if not we lost.
*************************************************************************/
int didLoose(){
    int i;
    for(i =0; i<SIZE;i++){
        if(numbers[i] == 0){
            return 0;
        }
    }
    return 1;
}

/***********************************************************************
 * function name: didWin
 * input: none
 * output: an int (if we won or not)
 * operation: checks if theres a 2048 on the board. if yes we won.
*************************************************************************/

int didWin(){
    int i;
    for(i =0; i<SIZE;i++){
        if(numbers[i] == 2048){
            return 1;
        }
    }
    return 0;
}

/***********************************************************************
 * function name: sendWonOrLost
 * input: the number (-1 or -2)
 * output: none
 * operation: sends the single number with a comma as a message. sends
 * with kill
*************************************************************************/
void sendWonOrLost(int i){
    char message[12];
    sprintf(message,"%i",i);
    strcat(message, ",");
    write(1,message, strlen(message));

    //send signal to process 1
    if(kill (pidOfProcess1,SIGUSR1)<0){
        perror("error!\n");
        exit(-1);
    }
    exit(1);
}

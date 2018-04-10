#include<unistd.h>
#include<stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <ctype.h>

typedef struct {
    int col;
    int row;
    int multiArray[3][3];
    bool result;

} sudokuBoard;


pthread_mutex_t lock;
 
 
 void*  rowCheck(void * board);
 void * colCheck(void * board);



int main(int argc, char* argv[]){
    FILE * inputFile = fopen(argv[1], "r");
    int curInt;

    sudokuBoard *newBoard = malloc(sizeof(sudokuBoard));
    newBoard->result = true;
   
       for(int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            curInt = fgetc(inputFile);
            if(isspace(curInt)){
                curInt = fgetc(inputFile);
            }

            newBoard->multiArray[i][j] = curInt;
        }
    }

   
    pthread_t *rowThread;
    pthread_t *colThread;
    rowThread = malloc( 9 * sizeof(pthread_t));
    colThread = malloc(9 * sizeof(pthread_t));
    
    for(int i = 0; i < 9; i++){
        pthread_create(&rowThread[i], NULL, rowCheck, (void * ) newBoard);
        pthread_create(&colThread[i], NULL, colCheck, (void * ) newBoard);
    }
    for(int i = 0; i < 9; i++){
        for(int j = 0; j <9; j++){
            newBoard->multiArray[i][j] = j;
        }
    }
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            printf("Hitting here.\n");
            printf("%d ", newBoard->multiArray[i][j]);
        }
        printf("\n");
    }
    bool rowRes;
    bool colres;
    for(int i = 0; i < 9; i++){
        pthread_join(rowThread[i], (void *) &rowRes);
        pthread_join(colThread[i], (void *) &colres);
    }

    free(rowThread);
    free(colThread);

    pthread_mutex_destroy(&lock);
    return 0;

    
}

void* rowCheck( void * board ){
    sudokuBoard *rowBoard = (sudokuBoard * ) board;

    printf("Entering rowCheck\n");
    rowBoard->row += 1;
    pthread_mutex_lock(&lock);
    int rowsum = 0; 

    for(int i = 0; i < 3; i++){
        
        rowsum += rowBoard->multiArray[rowBoard->row][i];
        printf("Rowsum: %d\n", rowsum);
    }
    if(rowsum != 3){
        rowBoard->result = false;
        printf("Error in row: %d", rowBoard->row);
    }
    printf("Good to go.\n");
    printf("Unlocking and Headed back to main thread.\n");
    pthread_mutex_unlock(&lock);
    return (void * )rowBoard->result;

}

void * colCheck(void * board){
    
    printf("Entering colCheck.\n");
    sudokuBoard *colBoard = (sudokuBoard *) board;
    // int count = colBoard->col + 1;
    colBoard->col += 1;
    int colsum = 0;
    pthread_mutex_lock(&lock);
    for(int i = 0; i < 3; i ++){
            colsum += colBoard->multiArray[i][colBoard->col];
            printf("Colsum: %d \n", colsum);
           }

        
    if(colsum != 3){ 
            printf("Error in column: %d", colBoard->col);
            colBoard->result = false;
    }
    printf("Good to go.\n Unlocking...\n");
    
    pthread_mutex_unlock(&lock);
    printf("Headed back to main thread.\n");
    return (void* )colBoard->result;
        
}


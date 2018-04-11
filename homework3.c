#include<unistd.h>
#include<stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <ctype.h>

typedef struct {
    int col;
    int row;
    int multiArray[9][9];
    bool result;

} sudokuBoard;


pthread_mutex_t lock;
 
 
 void*  rowCheck(void * board);
 void * colCheck(void * board);



int main(int argc, char* argv[]){
    int curInt;    
    pthread_t *rowThread;
    pthread_t *colThread;
    pthread_t *gridThread; // use this for grid threads
    void * rowRes;
    void *colres;
    void *gridres; // Use this like rowres and colres
 
    FILE * inputFile = fopen(argv[1], "r");
    sudokuBoard *newBoard = malloc(sizeof(sudokuBoard));


    pthread_mutex_init(&lock, NULL);
    
    newBoard->result = true;
    newBoard->row = 0;
    newBoard->col = 0;
   
       for(int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            curInt = fgetc(inputFile); // reading in ascii value
            if(isspace(curInt)){
                curInt = fgetc(inputFile);
                
            }
            curInt -= 48;

            newBoard->multiArray[i][j] = curInt;
        }
    }


    rowThread = malloc( 9 * sizeof(pthread_t));
    colThread = malloc(9 * sizeof(pthread_t));
    gridThread = malloc(9 * sizeof(pthread_t));
    
    for(int i = 0; i < 9; i++){
        pthread_create(&rowThread[i], NULL, rowCheck, (void * ) newBoard);
    }

    for(int i = 0; i < 9; i++){
        pthread_create(&colThread[i], NULL, colCheck, (void * ) newBoard);
    }

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            
            printf("%d ", newBoard->multiArray[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < 9; i++){
        pthread_join(rowThread[i], (void *) &rowRes);
        pthread_join(colThread[i], (void *) &colres);
    }
    
    if(newBoard->result == false){
        printf("This is not a suduko board.\n");
    }
    else{
        printf("This is a valid sudoku board.\n");
    }

    free(rowThread);
    free(colThread);
    // close(inputfile);
    pthread_mutex_destroy(&lock);
    return 0;

    
}

void* rowCheck( void * board ){
    sudokuBoard *rowBoard = (sudokuBoard * ) board;

    printf("Entering rowCheck\n");
    pthread_mutex_lock(&lock);
    int rowsum = 0; 

    for(int i = 0; i < 9; i++){
        
        rowsum = rowsum + rowBoard->multiArray[rowBoard->row][i];
        printf("Rowsum: %d\n", rowsum);
    }
            rowBoard->row += 1;

    if(rowsum != 45){
        rowBoard->result = false;
        printf("Error in row: %d.  Rowsum is: %d\n", rowBoard->row, rowsum);
        
    }
    // printf("Good to go.\n");
    printf("Unlocking and Headed back to main thread.\n");
    pthread_mutex_unlock(&lock);
    return (void * )rowBoard->result;

}

void * colCheck(void * board){
    
    printf("Entering colCheck.\n");
    sudokuBoard *colBoard = (sudokuBoard *) board;

    int colsum = 0;
    pthread_mutex_lock(&lock);
    for(int i = 0; i < 9; i ++){
            colsum = colsum + colBoard->multiArray[i][colBoard->col];
           
           }
     colBoard->col += 1;
        
    if(colsum != 45){ 
            printf("Error in column: %d", colBoard->col);
            colBoard->result = false;
    }
    printf("Unlocking...\n");
    
    pthread_mutex_unlock(&lock);
    printf("Headed back to main thread.\n");
    return (void* )colBoard->result;
        
}



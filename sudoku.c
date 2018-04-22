// Sean Moloney, Colin Franceschini
// CS450 Homework 3


// use this to compile on blue:
// gcc sudoku.c -o sudoku.x -lpthread

// run with: ./sudoku.x < ./input.txt


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <ctype.h>

typedef struct {
    int col;
    int row;
    int multiArray[9][9];
    int grid;
    bool result;

} sudokuBoard;


pthread_mutex_t lock;

void*  rowCheck(void * board);
void * colCheck(void * board);
void* gridCheck( void* B );


int main(int argc, char* argv[]){
    // initialize threads for row check, column check, and whole subgrid check
    pthread_t *rowThread;
    pthread_t *colThread;
    pthread_t *gridThread;
    void * rowRes;
    void *colres;
    void *gridres;

    // initialize board
    sudokuBoard *newBoard = malloc(sizeof(sudokuBoard));
    pthread_mutex_init(&lock, NULL);
    newBoard->result = true;
    newBoard->row = 0;
    newBoard->col = 0;

    int curr = 0;
    for(int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            curr = getc(stdin);
            if(isspace(curr)){
                curr = getc(stdin);
            }
            curr = curr - '0';
            newBoard->multiArray[i][j] = curr;
        }
    }

    int n1 = 0;
    int n2 = 0;
    // printing board
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            printf( "%d" , newBoard->multiArray[i][j] );
            printf(" ");
            if (n1 == 2) {
                printf("    ");
                n1 = 0;
            }else
                n1++;
        }
        printf("\n");
        if (n2 == 2) {
            printf("\n");
            n2 = 0;
        }else
            n2++;
    }
    rowThread = malloc( 9 * sizeof(pthread_t));
    colThread = malloc(9 * sizeof(pthread_t));
    gridThread = malloc(9 * sizeof(pthread_t));
    // create threads for each row check
    for(int i = 0; i < 9; i++)
        pthread_create(&rowThread[i], NULL, rowCheck, (void * ) newBoard);
    // create threads for each column check
    for(int i = 0; i < 9; i++)
        pthread_create(&colThread[i], NULL, colCheck, (void * ) newBoard);
    // create threads for each subgrid check
    int gridNum = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            pthread_create( &gridThread[gridNum], NULL, gridCheck, (void *) newBoard );
            gridNum +=1;
        }
    }
    // wait for threads to finish
    for(int i = 0; i < 9; i++){
        pthread_join(rowThread[i], &rowRes);
        pthread_join(colThread[i], &colres);
        pthread_join(gridThread[i], &gridres);
    }
    bool rowNotValid = (unsigned long)rowRes == 0;
    bool colNotValid = (unsigned long)colres == 0;
    bool gridNotValid = (unsigned long)gridres == 0;
    if( rowNotValid || colNotValid || gridNotValid )
        printf("\nThis is not a valid suduko board.\n");
    else
        printf("\nThis is a valid sudoku board.\n");
    free(rowThread);
    free(colThread);
    free(gridThread);
    pthread_mutex_destroy(&lock);
    printf( "\n");
    return 0;


}

// This function checks each row for validity
void* rowCheck( void * board ){
    pthread_mutex_lock(&lock);
    sudokuBoard *rowBoard = (sudokuBoard * ) board;
    int rowsum = 0;
    rowBoard->row += 1;
    // add up all the values of each column,
    // if the sum of each column is not 45, its not a valid column
    for(int i = 0; i < 9; i++)
        rowsum = rowsum + rowBoard->multiArray[rowBoard->row-1][i];

    if(rowsum != 45){
        rowBoard->result = false;
        printf("Row %d doesn't have the required values.\n", rowBoard->row);
    }
    pthread_mutex_unlock(&lock);
    return (void * )rowBoard->result;
}

// This function checks each column for validity
void * colCheck(void * board){
    pthread_mutex_lock(&lock);
    int colsum = 0;
    sudokuBoard *colBoard = (sudokuBoard*) board;
    colBoard->col +=1;
    // add up all the values of each column,
    // if the sum of each column is not 45, its not a valid column
    for( int i = 0; i < 9; i++)
        colsum = colsum + colBoard->multiArray[i][colBoard->col-1];
    if( colsum != 45 ){
        colBoard->result = 0;
        printf( "Column %d doesn't have the required values.\n", colBoard->col );
    }
    pthread_mutex_unlock(&lock);

    return (void *) colBoard->result;
}


// This function checks each subgrid for validity
void* gridCheck( void* B ) {
    pthread_mutex_lock(&lock);
    int sum = 0;
    sudokuBoard *gridBoard = (sudokuBoard*) B;
    gridBoard->grid += 1;
    // add up all the values of each subgrid,
    // if the sum of each subgrid is not 45, its not a valid subgrid
    for( int r = ((gridBoard->grid-1)/3)*3; r < ((gridBoard->grid-1)/3)*3 + 3; r++) {
        for( int c = ((gridBoard->grid-1)%3)*3; c < ((gridBoard->grid-1)%3)*3 + 3; c++)
            sum = sum + gridBoard->multiArray[r][c];
    }
    if( sum != 45 ) {
        gridBoard->result = false;
        switch( gridBoard->grid) {
            case 1:
                printf( "The top left subgrid doesn't have the required values");
                break;
            case 2:
                printf( "The top middle subgrid doesn't have the required values");
                break;
            case 3:
                printf( "The top right subgrid doesn't have the required values");
                break;
            case 4:
                printf( "The left middle subgrid doesn't have the required values");
                break;
            case 5:
                printf( "The center middle subgrid doesn't have the required values");
                break;
            case 6:
                printf( "The right middle subgrid doesn't have the required values");
                break;
            case 7:
                printf( "The bottom left subgrid doesn't have the required values");
                break;
            case 8:
                printf( "The bottom middle subgrid doesn't have the required values");
                break;
            case 9:
                printf( "The bottom right subgrid doesn't have the required values");
                break;
            default:
                break;
        }
    }
    pthread_mutex_unlock(&lock);
    return (void *) gridBoard->result;
}
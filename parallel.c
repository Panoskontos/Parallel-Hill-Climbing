#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define ROWS 10
#define COLS 10

int array[ROWS][COLS];
int visited[ROWS][COLS] = {0}; // Mark the visited cell


// algorithm to generate random number
int rand_range(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}



int main(int argc, char *argv[]) {
    int rank, num_procs, global_max,global_x,global_y;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Seed the random number generator
    srand(time(NULL) + rank);
    int i, j;
    int my_current_i, my_current_j, my_current_value;
    if (rank == 0) {  //Process 0 is responsible for generating the array.
        // Initialize the array with random values
        for (i = 0; i < ROWS; i++) {
            for (j = 0; j < COLS; j++) {
                array[i][j] = rand() % (COLS*ROWS);
            }
        }

        // print array
        for (i = 0; i < ROWS; i++) {
                    for (j = 0; j < COLS; j++) {
                        printf("%d " , array[i][j]);
                }
                    printf("\n");
                }

    }
    //Broadcast the array to all other processes
    MPI_Bcast(array, ROWS*COLS, MPI_INT, 0, MPI_COMM_WORLD);

// Seperate Array to portions
    int portion = ROWS / num_procs;
    int start = rank * portion;
    int end = start + portion;
printf("\nrank %d starts from row %d and end to row %d",rank,start,end);


// Random number inside the portion we declared
    // my_current_i = rand_range(start,end);
    // my_current_j = rand_range(0,COLS);
    // my_current_value = array[my_current_i][my_current_j];

// loop the hill climb not random times
int restarts =  4;
int local_max_from_restarts = -1;
j = 0;
while(j<restarts){
    //   Find the max local
int row = rand_range(start,end);
int col = rand_range(0,COLS);
printf("\nrank %d loop %d random spot is [%d %d]",rank,j,row,col);
int steps = 0;  // Use steps for load balancing 
int local_max=-1;
int local_x = 0;
int local_y =0;
// hill climbing algorithm with steps
while (1) {
        int currentValue = array[row][col];
        visited[row][col] = 1;
        int nextRow = row, nextCol = col;
        printf("\nChecking for at [%d, %d] with value %d\n", row, col, currentValue);
        // check the value of the neighboring cells
        // check for visited
        // check for boundaries of others
        if (row > 0 && array[row - 1][col] > currentValue&& !visited[row-1][col] && row-1 > start ) {
            nextRow = row - 1;
        }
        if (row < ROWS - 1 && array[row + 1][col] > currentValue && !visited[row+1][col] && row+1 < end ) {
            nextRow = row + 1;
        }
        if (col > 0 && array[row][col - 1] > currentValue&& !visited[row][col-1]) {
            nextCol = col - 1;
        }
        if (col < COLS - 1 && array[row][col + 1] > currentValue&& !visited[row][col+1]) {
            nextCol = col + 1;
        }

        // if no neighboring cell has a higher value, we've reached the peak
        if (nextRow == row && nextCol == col) {
            printf("\nReached peak at [%d, %d] with value %d\n", row, col, currentValue);
            local_max = currentValue;
            if(local_max>local_max_from_restarts){
                local_max_from_restarts=local_max;
            }
            local_x = row;
            local_y = col;
            break;
        }

        if(steps>=5){
             printf("\nNo more steps for rank %d stopped at [%d, %d] with value %d\n",rank, row, col, currentValue);
            local_max = currentValue;
             if(local_max>local_max_from_restarts){
                local_max_from_restarts=local_max;
            }
             local_x = row;
            local_y = col;
             break;
        }

        // otherwise, move to the neighboring cell with the highest value
        row = nextRow;
        col = nextCol;
        steps++;
    }
    j++;
}
 // printf("\nRank %d has local maximum with value %d\n", rank, local_max);
    MPI_Reduce(&local_max_from_restarts, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);


   
//   print visited
    // if(rank==1){
    //     for (i = 0; i < ROWS; i++) {
    //                 for (j = 0; j < COLS; j++) {
    //                     printf("%d " , visited[i][j]);
    //             }
    //                 printf("\n");
    //             }
    // }



if (rank == 0) {
        printf("\nThe global maximum is: %d\n", global_max);
        // printf("Time taken: %fs\n", end_time - start_time);
    }


    MPI_Finalize();
    return 0;
}
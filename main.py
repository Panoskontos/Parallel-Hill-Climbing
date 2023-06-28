import numpy as np
import random
from mpi4py import MPI
import time


ROWS = 1000
COLS = 1000
RESTARTS = 5
MAX_STEPS_EACH_CLIMB = 5

# function to create array of random numbers
def initilize_array(n,m):
    arr = np.random.randint(0,n*m,size=(n,m))
    return arr;

# hill climbing algorithm that starts from random
# spot and searches for a fixed number of iterations
def hill_climb(arr, max_iter = MAX_STEPS_EACH_CLIMB):
    # Create visited array
    visited = np.zeros(arr.shape, dtype=bool)
    # find rows and columns of the array
    rows = len(arr)
    cols = len(arr[0])
    # generate random starting point
    current = [random.randint(0,rows-2),random.randint(0,cols-2)]
    # set the initial starting position to visited
    visited[current[0]][current[1]] = True
    i=0
    while i<max_iter:
        i+=1
        # Get current value
        current_val = arr[current[0], current[1]]
        # Get possible movements
        movements = [[-1, 0], [1, 0], [0, -1], [0, 1]]
        # Find best neighbor
        best_neighbor = None
        best_neighbor_val = float("-inf")
        # test for all movements
        for movement in movements:
            neighbor = [current[0] + movement[0], current[1] + movement[1]]
            if (
                neighbor[0] < 0 or
                neighbor[0] >= arr.shape[0] or
                neighbor[1] < 0 or
                neighbor[1] >= arr.shape[1]
            ):
                continue
            if visited[neighbor[0], neighbor[1]]:
                continue
            if arr[neighbor[0], neighbor[1]] > best_neighbor_val:
                best_neighbor = neighbor
                best_neighbor_val = arr[neighbor[0], neighbor[1]]
        if best_neighbor is None:
            return current
        if best_neighbor_val <= current_val:
            return current
        # Update current and mark as visited
        current = best_neighbor
        # update the visited table
        visited[current[0], current[1]] = True


def parallel_hill_climbing():
    comm = MPI.COMM_WORLD
    size = comm.Get_size()
    rank = comm.Get_rank()

    if rank == 0:
        arr = initilize_array(ROWS,COLS)
        # print(arr)
    else:
        arr = None
    
    # when array is created we start the benchmarking 
    start_time = time.time()

    # broadcast array
    arr = comm.bcast(arr, root=0)
    # Divide array into chunks for each process
    process_arr = np.array_split(arr, size)[rank]
    # print('\n', process_arr ,'n')
    
    process_max = -1
    # split in n grids by column
    local_arr = np.hsplit(process_arr, RESTARTS)
    i = 0
    while(i<len(local_arr)):
        # Compute max of local array with hill climbing
        local_max = hill_climb(local_arr[i],MAX_STEPS_EACH_CLIMB)
        # get max value 
        local_max = local_arr[i][local_max[0],local_max[1]]
        # get the largest from all splits
        if(local_max>process_max):
            process_max=local_max
        i+=1
    # print("rank",rank, " has max of ", process_max)

    # Reduce local max to get global max
    global_max = comm.reduce(process_max, op=MPI.MAX, root=0)
    if rank == 0:
        print("Global max:", global_max)
        end_time = time.time()
        execution_time = end_time - start_time
        print("My Execution time:", execution_time,'sec') 
       
   
    MPI.Finalize()



if __name__ == "__main__":
    parallel_hill_climbing()

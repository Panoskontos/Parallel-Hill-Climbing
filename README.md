# Parallel-Hill-Climbing

The task involves implementing a parallel algorithm using the MPI programming model to find the maximum value of a function with two variables. Here are the key points:

- What is the most efficient way to find the tallest point?

There are many answers to that my approach was to divide the area into a grid, like a big square with smaller squares inside. Each square represents a combination of two numbers that we can input into the function.

Now, instead of checking every single combination of numbers in the grid, which would take a long time, we use a clever strategy. We start at a random square in the grid and check its value using the function. Then, we look at the four neighboring squares and go to the one with the highest value. We keep doing this, moving to squares with higher values, until we can't find any neighboring square with a higher value.

Sometimes, we might get stuck at a square where all the neighboring squares have lower values. In that case, we know we have found the highest value we can get in that area.

But here's the interesting part: instead of starting our search from just one random square, we start from multiple random squares in different parts of the grid. This increases our chances of finding the absolute highest value for the function in the whole area.

To make things even more efficient, we remember the values we have already checked so that we don't repeat the same calculations. This saves time and helps us focus on exploring new squares.

Now, if we want to speed things up and have multiple people working on this task, we can divide the grid among them. Each person takes care of a specific part of the grid and looks for the highest value there. This way, we can finish the task faster by working together.




![image I2GT61](https://github.com/Panoskontos/Parallel-Hill-Climbing/assets/65974766/90cae5ea-8422-4be7-8d70-a29ed5bb8969)



![image TGL161](https://github.com/Panoskontos/Parallel-Hill-Climbing/assets/65974766/92f7a02e-1b38-4f7f-990b-f758488542a3)


</br></br>

Results (We have more accuracy for the highest point with almost the same time) 


![image LFR661](https://github.com/Panoskontos/Parallel-Hill-Climbing/assets/65974766/0c0b83c5-dac8-4c29-896d-2f0c0eb777f7)

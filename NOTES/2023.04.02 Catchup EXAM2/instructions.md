Write a C++ program that creates multiple POSIX threads to print the value of a positive integer value (from STDIN) in reverse order. The program executes the following tasks:

    1. Receives as input a string representing a positive integer value number.
    2. Creates "nthreads" child threads (where "nthreads" is the number of digits of the positive integer value from the input).
    3. Assigns a thread number to each child thread based on the order they are created. The value for the thread number must start from zero and be increased by one before assigning it to the next child thread. You must guarantee that the first child thread has a thread number equal to zero, the second child thread has a thread number equal to one, and the nth child thread has a thread number equal to n-1.
    4. Assigns a digit from the input value to each child thread. The child threads receive the digit from the input based on their thread number (from left to right); Given the input value "4723", the first child thread receives the digit "4", the second child thread receives the digit "7", the third child thread receives the digit "2", and the fourth child thread receives the digit "3".
    5. Each child thread prints the received digit from the main thread. The print statement executed by the child thread cannot be inside a ciritcal section.
    6. Your program must guarantee that the digits are printed from the least significant to the most significant (in reverse order).

For a positive integer value = 4723, the expected output is:

3274

NOTES

    1. You must create n child threads (where n is the number of digits from the input value).
    2. You can only use POSIX semaphores, pthreads mutex semaphores, or pthreads condition variables to achieve synchronization (A penalty of 100% will be applied to solutions using mechanisms other than the synchronization mechanisms listed before).
    3. A penalty of 100% will be applied to solutions that do not assign the "threadNumber" based on the specifications of the question.
    4. Programs not compiling will receive a penaly of 100%.
    5. To simplify your solution, the template file defines several global variables that you are allowed to use in your program for this programming question. However, remember that you should avoid using global variables when writing programs.
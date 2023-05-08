/*
Synchronization concepts
High Level Synchronization
1) Barrier Synchronization -> Each thread will wait at barrier until all the threads arrive
#pragma omp barrier -> Puts a barrier in the pgm, so that all threads will reach the point before any thread proceeds to next line

2) Mutual Exclusion -> Define a block of code that only one thread can execute at any instant
#pragma omp critical {...}
Try to keep the content of critical section very limited as they will be sequential

3) Atomic -> Similar to CS, but uses hardware constructs to execute in atomically
Use it for very small operations that needs to be mutually exclusive
#pragma omp atomic {...}

*/

/*
This program will numerically compute the integral of
              4/(1+x*x)

from 0 to 1.  The value of this integral is pi.
*/

#include <stdio.h>
#include <omp.h>
static long num_steps = 100000000;
double step;
const static int NUM_THREADS = 10;

const static int TEST_SIZE = 100;

double pi_parallel();

int main()
{
    long double total_time = 0.0;
    for (int i = 0;i<TEST_SIZE;i++)
        total_time = total_time + pi_parallel();
    printf("Average time taken : %Lf", total_time/TEST_SIZE);
    
}

double pi_parallel()
{

    double pi, start_time, run_time;
    int num_threads_allocated;
    step = 1.0 / (double)num_steps;
    start_time = omp_get_wtime();
    omp_set_num_threads(NUM_THREADS);
#pragma omp parallel
    {
        int i, n_threads;
        double x, sum_private = 0.0;
        int thread_num = omp_get_thread_num();
        n_threads = omp_get_num_threads();
        if (thread_num == 0)
            // This is done as the number of threads allocated can vary at runtime.
            // omp_get_thread_num -> Will only specify the upper bound on the number of threads that can be allocated
            num_threads_allocated = n_threads;
        int step_start = (num_steps / n_threads) * thread_num;
        int end_step = ((num_steps / n_threads) * (thread_num + 1)) - 1;
        // printf("Thread number : %d ; Start step : %d ; End step : %d\n",thread_num, step_start, end_step);

        for (i = step_start; i <= end_step; i++)
        {
            x = (i - 0.5) * step;
            sum_private = sum_private + 4.0 / (1.0 + x * x);
        }

        #pragma omp critical
            pi = pi + step * sum_private;
    }
    run_time = omp_get_wtime() - start_time;
    // printf("\n pi with %ld steps is %lf in %lf seconds\n ", num_steps, pi, run_time);
    return run_time;
}
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

void pi_sequential();
void pi_parallel();

int main()
{
    pi_sequential();
    pi_parallel();
}

void pi_sequential()
{
    int i;
    double x, pi, sum = 0.0;
    double start_time, run_time;

    step = 1.0 / (double)num_steps;

    start_time = omp_get_wtime();

    for (i = 1; i <= num_steps; i++)
    {
        x = (i - 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }

    pi = step * sum;
    run_time = omp_get_wtime() - start_time;
    printf("\n pi with %ld steps is %lf in %lf seconds\n ", num_steps, pi, run_time);
}

void pi_parallel()
{

    double pi, sum[NUM_THREADS], start_time, run_time;
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
        sum[thread_num] = sum_private;
    }

    for (int j = 0; j < num_threads_allocated; j++)
        pi = pi + step * sum[j];
    run_time = omp_get_wtime() - start_time;
    printf("\n pi with %ld steps is %lf in %lf seconds\n ", num_steps, pi, run_time);
}
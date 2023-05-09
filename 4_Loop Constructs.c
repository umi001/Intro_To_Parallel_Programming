/*
Work Sharing Constructs
1) Loop Construct
2) Section Construct
3) Single Construct
4) Task Construct

1) Loop Construct
#pragma omp parallel{
    #pragma omp for  // This means take the immediately following for loop and split it up iterations into threads
        for (i = 0; i<N ; i++) {
            fun(i);
        }
}

shorter version :
#pragma omp parallel for
        for (i = 0; i<N ; i++) {
            fun(i);
        }

Schedules :
Scheduling is a method in OpenMP to distribute iterations to different threads in for loop.

The basic form of OpenMP scheduling is

#pragma omp parallel for schedule(scheduling-type, chunk-size) 
for(conditions){
    do something
}


1) Static (default) -> Defined at run time
2) Dynamic -> OpenMP will still split task into iter_size/chunk_size chunks, but distribute trunks to threads dynamically without any specific order.
3) Guided -> 
4) Runtime -> depends on env variable
5) Auto -> Will delegates the decision of the scheduling to the compiler and/or runtime system. That means, scheduling will be decided automatically by your machine.

REFERENCE : https://610yilingliu.github.io/2020/07/15/ScheduleinOpenMP/
*/
#include <stdio.h>
#include <omp.h>
static long num_steps = 1000000000;
const static int NUM_THREADS = 10;
double step;

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
    int i;
    double pi, sum = 0.0;
    double start_time, run_time;

    step = 1.0 / (double)num_steps;

    start_time = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum) // schedule(dynamic) // This will specify the type of schedule to use : static by default
        for (i = 1; i <= num_steps; i++)
        {
            double x = (i - 0.5) * step;
            sum = sum + 4.0 / (1.0 + x * x);
        }

    pi = step * sum;
    run_time = omp_get_wtime() - start_time;
    printf("\n pi with %ld steps is %lf in %lf seconds\n ", num_steps, pi, run_time);
}

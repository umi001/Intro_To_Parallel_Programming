#include<omp.h>
#include <stdio.h>


int main()
{
#pragma omp parallel
	{
		/*
		 * Eveything inside this block will be a executed as a separate thread. This block will be executed in parallel.
		 * The number of threads created depend on the value of environment variable OMP_NUM_THREADS
		*/

		printf("Hello World\n");
	}

}


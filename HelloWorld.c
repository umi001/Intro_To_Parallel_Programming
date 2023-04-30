#include <omp.h>
#include <stdio.h>

int main()
{
#pragma omp parallel
	{
		/*
		 * Eveything inside this block will be a executed as a separate thread. This block will be executed in parallel.
		 * The number of threads created depend on the value of environment variable OMP_NUM_THREADS
		 */
		int id = omp_get_thread_num(); // Return a unique identifier for each thread

		printf("Hello %d", id);
		printf("World %d\n", id);
	}
}


/*
Sample output 
Hello 0Hello 4World 4
Hello 2World 2
World 0
Hello 5Hello 3World 5
Hello 6Hello 9World 6
Hello 1Hello 8World 8
World 3
World 1
Hello 7World 7
World 9	
*/
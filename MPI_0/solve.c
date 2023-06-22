#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define PI 3.14159265358979323846

int main(int argc, char **argv)
{
	int rank, size;
	int i;
	double pi = .0, global_pi = .0, x;
	const int n = 1e6;
	//SeqMethod
	int sum = 0;
	double startPoint, endPoint;
	startPoint = MPI_Wtime();
	for (int i = 0; i < n; ++i)
	{
		x = (i + 0.5) / n;
		sum += 4 / (1 + x*x);
	}
	endPoint = MPI_Wtime();
	MPI_Init(NULL, NULL);

	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Barrier(MPI_COMM_WORLD);
	double start, end;
	start = MPI_Wtime();

	for(i = rank; i < n; i +=size)
	{
		x = (i + 0.5) / n;
		pi += 4 / (1 + x*x);
	}

	MPI_Reduce(&pi, &global_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	end = MPI_Wtime();

	if(rank == 0)
	{
		global_pi /= n;

		printf("Precision: %u \n", n);
		printf("PI: %9.8f \n", PI);
		printf("Calculated: %9.8f \n", global_pi);
		printf("Difference: %e \n", PI - global_pi);
		printf("SeqTime(ms): %f \n", (endPoint - startPoint) * 1000);
		printf("ParTime(ms): %f \n", (end - start) * 1000);
	}

	MPI_Finalize();
	return 0;
}

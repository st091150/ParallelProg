#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 512

int main()
{
	int rank, size;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int taskCount = N * N / size;
	int a[N*N], b[N*N], c_seq[N*N];
	int a_buf[taskCount], c_buf[taskCount], c_par[N*N];

	if(rank == 0)
	{
		srand(time(NULL));
		for(int i = 0; i < N; ++i)
		{
			for(int j = 0; j < N; ++j)
			{
				a[i*N+j] = rand() % 11;
				b[i*N+j] = rand() % 11;
				c_seq[i*N+j] = c_par[i*N+j] = 0;
			}
		}
		for(int i = 0; i < taskCount; ++i)
		{
			c_buf[i] = 0;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double start = MPI_Wtime();
	
	MPI_Scatter(&a, taskCount, MPI_INT, &a_buf, taskCount, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&b, N*N, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&c_buf, taskCount, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&c_par, N*N, MPI_INT, 0, MPI_COMM_WORLD);

	for(int i =0; i < taskCount/N; ++i)
	{
		for(int j = 0; j < N; ++j)
		{
			for(int k = 0; k < N; ++k)
			{
				c_buf[i*N+j] += a_buf[i*N+k] * b[k*N+j];
			}
		}
	}

	MPI_Gather(&c_buf, taskCount, MPI_INT, &c_par, taskCount, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	double end = MPI_Wtime();

	if(rank == 0)
	{
		double startPoint = MPI_Wtime();
		for(int i = 0; i < N; ++i)
		{
			for(int j =0; j < N; ++j)
			{
				for(int k = 0; k < N; ++k)
				{
					c_seq[i*N+j] += a[i*N+k] * b[k*N+j];
				}
			}
		}
		double endPoint = MPI_Wtime();
		
		printf("SeqMethod\nTime(ms) %f\n", (endPoint - startPoint) * 1000);
		printf("ParMethod\nTime(ms) %f\n", (end - start) * 1000);
	}
	MPI_Finalize();
	return 0;
}

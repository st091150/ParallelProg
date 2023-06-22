#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void merge(int *a, int *b, int l, int m, int r)
{
	int h, i, j, k;
	h = i = l;
	j = m + 1;

	while((h <=m) && (j <= r))
	{
		if(a[h] <= a[j])
		{
			b[i] = a[h];
			h++;
		}
		else
		{
			b[i] = a[j];
			j++;
		}
		i++;
	}
	if(m < h)
	{
		for(k = j; k <= r; k++)
		{
			b[i] = a[k];
			i++;
		}
	}
	else
	{
		for(k = h; k <= m; k++)
		{
			b[i] = a[k];
			i++;
		}
	}
	for(k = l; k <=r; k++)
	{
		a[k] = b[k];
	}
}

void mergeSort(int *a, int *b, int l, int r)
{
	int m;
	if(l< r)
	{
		m = (l+r)/2;

		mergeSort(a, b, l, m);
		mergeSort(a, b, (m+1), r);
		merge(a, b, l, m, r);
	}
}

int main()
{
	int N = 1e6;
	int *arr = malloc(N*sizeof(int));

	int c;
	srand(time(NULL));
	
	int rank, size;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int taskCount = N/size;
	int *sub_arr = malloc(taskCount * sizeof(int));
	
	MPI_Barrier(MPI_COMM_WORLD);
	double start = MPI_Wtime();
	MPI_Scatter(arr, taskCount, MPI_INT, sub_arr, taskCount, MPI_INT, 0, MPI_COMM_WORLD);
	int *tmp_arr = malloc(taskCount * sizeof(int));
	mergeSort(sub_arr, tmp_arr, 0, (taskCount - 1));

	int *sorted=NULL;
	if(rank == 0)
	{
		sorted = malloc(N*sizeof(int));
	}
	MPI_Gather(sub_arr, taskCount, MPI_INT, sorted, taskCount, MPI_INT, 0, MPI_COMM_WORLD);
	double end = MPI_Wtime();

	int *sup_arr = malloc(N * sizeof(int));
	int *sorted_seq = malloc(N*sizeof(int));
	MPI_Barrier(MPI_COMM_WORLD);	
	double startPoint = MPI_Wtime();
	mergeSort(sorted_seq, sup_arr, 0, N - 1);
	double endPoint = MPI_Wtime();


	if(rank == 0)
	{
		int *arr2 = malloc(N * sizeof(int));
		mergeSort(sorted, arr2, 0, (N-1));
		printf("ParMethod\nTime(ms): %f\n", (end - start) * 1000);
		printf("SeqMethod\nTime(ms): %f\n", (endPoint - startPoint) * 1000);
		free(sorted);
		free(arr2);
	}
	free(sup_arr);
	free(sorted_seq);
	free(arr);
	free(sub_arr);
	free(tmp_arr);

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return 0;
}

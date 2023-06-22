#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <ctime>
using namespace std;

void randMat(int** mat, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            mat[i][j] = rand() % 11;
        }
    }

    return;
}

int main(int argc, char** argv) {

    srand(time(NULL));
    int n1 = 1000, m1 = 500, n2 = n1, m2 = 1000;

    int** mat1, **mat2, **res;

    mat1 = new int*[n1];
    mat2 = new int* [n2];
    res = new int* [n1];

    for (int i = 0; i < n1; i++) {
        mat1[i] = new int[m1];
        mat2[i] = new int[m2];
        res[i] = new int[m2];
    }

    randMat(mat1, n1, m1);
    randMat(mat2, n2, m2);

    omp_set_num_threads(4);

    int sup = 5;
    int i, j, k;
    double startPoint, endPoint, avgTime = 0;

    for (int i = 0; i < sup; ++i)
    {
        startPoint = omp_get_wtime();
        for (i = 0; i < n1; i++) {
            for (j = 0; j < m2; j++) {
                res[i][j] = 0;
                for (k = 0; k < m1; k++) {
                    res[i][j] += (mat1[i][k] * mat2[k][j]);
                }
            }
        }
        endPoint = omp_get_wtime();
        avgTime += endPoint - startPoint;
    }
    std::cout << "seqMatMul\navgTime: " << avgTime / sup << std::endl;


    avgTime = 0;
    for (int i = 0; i < sup; ++i)
    {
        startPoint = omp_get_wtime();
#pragma omp parallel for shared(mat1, mat2, res) private(i, j, k)
        for (i = 0; i < n1; i++) {
            for (j = 0; j < m2; j++) {
                res[i][j] = 0;
                for (k = 0; k < m1; k++) {
                    res[i][j] += (mat1[i][k] * mat2[k][j]);
                }
            }
        }
        endPoint = omp_get_wtime();
        avgTime += endPoint - startPoint;
    }
    std::cout << "parMatMul\navgTime: " << avgTime / sup << std::endl;

    return 0;
}

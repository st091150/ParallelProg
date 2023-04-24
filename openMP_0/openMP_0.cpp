#include <iostream>
#include <omp.h>

#define ull unsigned long long

int main() {
    ull n = 1e8;
    ull sum = 0;
    ull* A = new ull[n];

    double avgTime = 0.;
    unsigned sup = 5;

    for (ull i = 0; i < n; ++i)
        A[i] = i;

    omp_set_num_threads(4);

    double startPoint, endPoint;
    for (int i = 0; i < sup; ++i)
    {
        startPoint = omp_get_wtime();
        for (ull i = 0; i < n; ++i)
        {
            sum += A[i];
        }
        endPoint = omp_get_wtime();

        avgTime += endPoint - startPoint;
    }

    std::cout << "SeqMethod "<< "avgTime :" << avgTime/sup << "sec" << std::endl;

    for (int i = 0; i < sup; ++i)
    {
        startPoint = omp_get_wtime();
        #pragma omp parallel for
        for (ull i = 0; i < n; ++i)
        {
            #pragma omp atomic
            sum += A[i];
        }
        endPoint = omp_get_wtime();

        avgTime += endPoint - startPoint;
    }

    std::cout << "ParAtomMethod " << "avgTime :" << avgTime / sup << "sec" << std::endl;

    sum = 0, avgTime = 0;

    for (int i = 0; i < sup; ++i)
    {
        sum = 0;

        startPoint = omp_get_wtime();
        #pragma omp parallel for reduction(+:sum)
        for (ull i = 0; i < n; ++i)
        {
            sum += A[i];
        }
        endPoint = omp_get_wtime();

        avgTime += endPoint - startPoint;
    }

    std::cout << "ParRedMethod " << "avgTime :" << avgTime / sup << "sec" << std::endl;
    delete[] A;

    return 0;
}


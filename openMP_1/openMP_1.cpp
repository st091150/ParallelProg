#include <iostream>
#include <omp.h>

#define ull unsigned long long
int main()
{
    ull N = 1e6;
    double x, sum, startPoint, endPoint, avgTime = 0;

    unsigned sup = 5;

    omp_set_num_threads(4);
    std::cout.precision(10);

    
    for (int i = 0; i < sup; ++i)
    {
        sum = 0;
        startPoint = omp_get_wtime();
        for (int j = 0; j < N; ++j)
        {
            x = (j + 0.5) / N;
            sum += 4 / (1 + x * x);
        }
        endPoint = omp_get_wtime();
        avgTime += endPoint - startPoint;
        sum /= N;
    }
    
    std::cout << "SeqMethod :\n" << "result :" << sum << "\navgTime :" << avgTime/sup << "sec" << std::endl;

    avgTime = 0;

    for (int i = 0; i < sup; ++i)
    {
        sum = 0;
        startPoint = omp_get_wtime();
        #pragma omp parallel for private(x) reduction(+:sum)
        for (int j = 0; j < N; ++j)
        {
            x = (j + 0.5) / N;
            sum += 4 / (1 + x * x);
        }
        endPoint = omp_get_wtime();
        avgTime += endPoint - startPoint;
        sum /= N;
    }
    
    std::cout << "\nParMethod :\n" << "result :" << sum << "\navgTime :" << avgTime/sup  << "sec" << std::endl;

    return 0;
}


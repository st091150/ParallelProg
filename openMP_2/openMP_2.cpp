#include <iostream>
#include <omp.h>
#include <vector>
#include <math.h>
#define ull unsigned long long

void foo(unsigned num_threads, ull N, unsigned cIter) {
    double sum, x, avgTime = 0;
    double startPoint, endPoint;

    for (int i = 0; i < cIter; ++i)
    {
        sum = 0;
        startPoint = omp_get_wtime();
        #pragma omp parallel for num_threads(num_threads) private(x) reduction(+:sum)
        for (int j = 0; j < N; ++j)
        {
            x = (j + 0.5) / N;
            sum += 4 / (1 + x * x);
        }
        endPoint = omp_get_wtime();
        avgTime += endPoint - startPoint;
        sum /= N;
    }
    std::cout << num_threads << '\t' << std::log10(N) << '\t' << avgTime / cIter << '\t' << sum << std::endl;
}

int main()
{
    ull N_1 = 1e2, N_2  = 1e6;

    std::cout.precision(10);
    std::vector<int> arr = { 1, 2, 4, 8, 10, 12 };
    
    for (int i = 0; i < arr.size(); ++i) {
        foo(arr[i], N_1, 12);
        std::cout << std::endl;
    }

    for (int i = 0; i < arr.size(); ++i) {
        foo(arr[i], N_2, 12);
        std::cout << std::endl;
    }

    return 0;
}


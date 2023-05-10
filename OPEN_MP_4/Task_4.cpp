#include <omp.h>
#include <stdint.h>

#include <algorithm>
#include <ctime>
#include <iostream>
#include <vector>

template <class ForwardIt>
void quicksort(ForwardIt first, ForwardIt last) {
    if (first == last) return;
    auto pivot = *std::next(first, std::distance(first, last) / 2);
    auto middlel = std::partition(
	first, last, [pivot](const auto& em) { return em < pivot; });
    auto middle2 = std::partition(
	first, last, [pivot](const auto& em) { return !(pivot < em); });
#pragma omp task
    quicksort(first, middlel);
#pragma omp task
    quicksort(middle2, last);
}

int main() {
    uint32_t ull = 1e7;
    double startPoint, endPoint, avgTime = 0;
    std::vector<unsigned long long> arr(ull);
	srand(time(NULL));

    for (uint32_t i = 0; i < ull; ++i) {
		arr[i] = rand() % 2500;
    }

    for (int i = 0; i < 5; ++i) {
		startPoint = omp_get_wtime();

		quicksort(arr.begin(), arr.end());

		endPoint = omp_get_wtime();
		avgTime += endPoint - startPoint;
    }
    std::cout << "Time:" << avgTime << '\n';
    avgTime = 0;
    for (int i = 0; i < 5; ++i) {
		startPoint = omp_get_wtime();
#pragma omp parallel num_threads(4)
	{
#pragma omp single
	    quicksort(arr.begin(), arr.end());
	}
		endPoint = omp_get_wtime();
		avgTime += endPoint - startPoint;
	}
    bool isSorted = std::adjacent_find(std::begin(arr), std::end(arr),
                        std::greater<int>()) == std::end(arr);
	std::cout << "Time:" << avgTime << ' ' << isSorted <<  '\n';
}

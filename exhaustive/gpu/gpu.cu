#include <iostream>
#include <chrono>
#include <thrust/device_vector.h>

int main() {
    auto startTime = std::chrono::steady_clock::now();

    int num_movies, num_categories;
    std::cin >> num_movies >> num_categories;

    std::vector<int> h_start_times(num_movies);
    std::vector<int> h_end_times(num_movies);
    std::vector<int> h_categories(num_movies);
    std::vector<int> h_L(num_categories);

    for (int i = 0; i < num_movies; i++) {
        std::cin >> h_start_times[i] >> h_end_times[i] >> h_categories[i];
        if (h_end_times[i] < h_start_times[i]) {
            h_end_times[i] = 24;
        }
        if (h_end_times[i] == h_start_times[i]) {
            h_end_times[i] += 1;
        }
        h_categories[i]--;  // convert categories to 0-based index
    }

    for (int i = 0; i < num_categories; i++) {
        std::cin >> h_L[i];
    }

    // Copy host vectors to device
    thrust::device_vector<int> start_times = h_start_times;
    thrust::device_vector<int> end_times = h_end_times;
    thrust::device_vector<int> categories = h_categories;
    thrust::device_vector<int> L = h_L;

    thrust::device_vector<int> dp((num_movies+1) * (num_categories+1), 0);

    for (int i = 1; i <= num_movies; i++) {
        for (int j = 1; j <= num_categories; j++) {
            int max_count = 0;
            for (int k = 0; k < i; k++) {
                if (categories[k] == j-1 && end_times[k] <= start_times[i-1] && dp[k * (num_categories+1) + j-1] + 1 <= L[j-1]) {
                    max_count = std::max(max_count, (int)dp[k * (num_categories+1) + j-1] + 1);
                } else {
                    max_count = std::max(max_count, (int)dp[k * (num_categories+1) + j]);
                }
            }
            dp[i * (num_categories+1) + j] = max_count;
        }
    }

    int max_count = 0;
    for (int j = 1; j <= num_categories; j++) {
        max_count = std::max(max_count, (int)dp[num_movies * (num_categories+1) + j]);
    }

    std::cout << "\nNúmero de filmes: " << max_count << "\n";

    auto endTime = std::chrono::steady_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    std::cout << std::fixed;
    std::cout << "Time elapsed during the brute-force algorithm (in microseconds): " << duration << std::endl;
    std::cout << std::scientific;

    return 0;
}

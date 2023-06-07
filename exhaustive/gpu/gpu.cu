// Include the necessary libraries
#include <thrust/host_vector.h>
#include <thrust/device_vector.h>
#include <thrust/functional.h>
#include <thrust/transform.h>
#include <thrust/fill.h>
#include <thrust/copy.h>
#include <iostream>
#include <cassert>
#include <random>
#include <chrono>
#include <math.h>

using namespace std;

// Define a Movie struct which contains start, end, and category of a movie
struct Movie {
  int start;
  int end;
  int category;
};

// Define a struct that applies the schedule calculation logic on each combination
struct ScheduleFunctor {
  int movieCount;  // Number of movies
  int categoryCount;  // Number of categories
  int *categoryLimits;  // Limits for each category
  Movie *movies;  // List of movies

  // ScheduleFunctor constructor
  ScheduleFunctor(int movieCount, Movie *movies, int categoryCount, int *categoryLimits)
      : movieCount(movieCount), movies(movies), categoryCount(categoryCount), categoryLimits(categoryLimits) {}

  // Functor function that calculates the number of movies that can be scheduled for a given combination
  __device__ __host__
  int operator()(int combination) {
    bool timeSlots[24] = {false};  // Available time slots

    // Copy the category limits to a local array
    int localCategoryLimits[20];
    for (int i = 0; i <= categoryCount; i++) {
      localCategoryLimits[i] = categoryLimits[i];
    }

    int scheduledMovies = 0;  // Counter for scheduled movies
    for (int i = 0; i < movieCount; i++) {
      if (combination & (1 << i)) {  // Check if the i-th movie is included in the combination
        Movie& currentMovie = movies[i];

        // Check if time slots for the movie's duration are occupied
        for (int j = currentMovie.start; j < currentMovie.end; j++) {
          if (timeSlots[j]) return -1;  // If a time slot is occupied, return -1
          timeSlots[j] = true;  // Mark time slot as occupied
        }
        // If category limit is reached, return -1
        if (localCategoryLimits[currentMovie.category] == 0) return -1;
        localCategoryLimits[currentMovie.category]--;  // Decrease the limit for the movie's category
        scheduledMovies++;
      }
    }
    return scheduledMovies;
  }
};

// Main function
int main(int argc, char *argv[]) {
  // Get number of movies and categories from the user
  int num_movies, num_categories;
  cin >> num_movies >> num_categories;

  // Get the limit for each category
  vector<int> category_limit(num_categories + 1);
  for (int i = 1; i <= num_categories; i++) {
    cin >> category_limit[i];
  }

  // Get the movie details
  vector<Movie> movies(num_movies);
  for (int i = 0; i < num_movies; i++) {
    cin >> movies[i].start >> movies[i].end >> movies[i].category;
    if (movies[i].end < movies[i].start) {
      movies[i].end = 24;
    }

    if (movies[i].end == movies[i].start) {
      movies[i].end += 1;
    }
  }

  // Start the timer
  auto startTime = chrono::steady_clock::now();
  
  // Move data to GPU
  int *category_limit_gpu;
  Movie *movies_gpu;
  cudaMalloc(&category_limit_gpu, category_limit.size() * sizeof(int));
  cudaMalloc(&movies_gpu, movies.size() * sizeof(Movie));
  cudaMemcpy(category_limit_gpu, category_limit.data(), category_limit.size() * sizeof(int), cudaMemcpyHostToDevice);
  cudaMemcpy(movies_gpu, movies.data(), movies.size() * sizeof(Movie), cudaMemcpyHostToDevice);
  
  thrust::device_vector<int> movie_counts(pow(2, movies.size()));
  thrust::counting_iterator<int> combinations(0);

  ScheduleFunctor functor(
    movies.size(), 
    movies_gpu, 
    num_categories, 
    category_limit_gpu
  );

  // Apply Functor to each element in the vector
  thrust::transform(combinations, combinations + pow(2, movies.size()), movie_counts.begin(), functor);

  // Find the maximum number of movies
//   int max_movies = thrust::reduce(movie_counts.begin(), movie_counts.end(), thrust::maximum<int>())
  int max_movies = *thrust::max_element(movie_counts.begin(), movie_counts.end());

  // Print the result
  cout << "\nNúmero de filmes: " << max_movies << "\n";
  
  // Stop the timer and calculate the elapsed time
  auto endTime = chrono::steady_clock::now();
  double duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

  // Print the elapsed time
  cout << fixed;
  cout << "Time elapsed during the brute-force algorithm (in microseconds): " << duration << endl;
  cout << scientific;

  cudaFree(category_limit_gpu);
  cudaFree(movies_gpu);

  return 0;
}

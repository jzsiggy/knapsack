#include <iostream>
#include <vector>
#include <bitset>

using namespace std;

// Define a struct to represent a movie
struct Movie {
    int start_time;
    int end_time;
    int category;
};

int main() {
    // Start the timer
    auto startTime = chrono::steady_clock::now();

    int num_movies, num_categories;
    cin >> num_movies >> num_categories;

    // Read the category limits into a vector
    vector<int> category_limit(num_categories + 1);
    for (int i = 1; i <= num_categories; i++) {
        cin >> category_limit[i];
    }

    // Read the movie information into a vector
    vector<Movie> movies(num_movies);
    for (int i = 0; i < num_movies; i++) {
        cin >> movies[i].start_time >> movies[i].end_time >> movies[i].category;
        if (movies[i].end_time < movies[i].start_time) {
            movies[i].end_time = 24;
        }
    }

    // Sort the movies by end time
    sort(movies.begin(), movies.end(), [](const Movie& a, const Movie& b) {
        if (a.end_time == b.end_time) { return a.start_time < b.start_time; }
        return a.end_time < b.end_time;
    });

    // // Sort the movies by start time
    // sort(movies.begin(), movies.end(), [](const Movie& a, const Movie& b) {
    //     return a.start_time < b.start_time;
    // });

    // Define a bitset to represent the availability of each hour of the day
    bitset<24> hours;

    // Keep track of how many movies of each category have been watched
    vector<int> num_watched(num_categories + 1, 0);

    // Keep track of the indices of the watched movies
    vector<int> watched;

    // Iterate over the movies and check if each movie can be watched
    for (int i = 0; i < num_movies; i++) {
        const Movie& movie = movies[i];

        // Check if any hour of the movie overlaps with an already watched movie
        bool can_watch = true;
        for (int hour = movie.start_time; hour < movie.end_time; hour++) {
            if (hours[hour]) {
                can_watch = false;
                break;
            }
        }
        if (hours[movie.start_time]) { can_watch = false; }

        // If the movie can be watched and its category limit has not been reached,
        // mark the hours as watched, add the movie to the list of watched movies,
        // and print the start time, end time, and category of the movie
        if (can_watch && num_watched[movie.category] < category_limit[movie.category]) {
            num_watched[movie.category]++;
            watched.push_back(i);
            hours[movie.start_time] = true;
            for (int hour = movie.start_time; hour < movie.end_time; hour++) {
                hours[hour] = true;
            }
            cout << movie.start_time << " " << movie.end_time << " " << movie.category << endl;
        }
    }

    // Output the number of watched movies
    cout << "Número de filmes: " << watched.size() << endl;

    // Calculate the time elapsed during algorithm execution
    auto endTime = chrono::steady_clock::now();
    double duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    cout << "Time elapsed during the aleatory algorithm: " << duration << " microseconds" << endl;

    return 0;
}

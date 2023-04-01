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

bool check_can_watch(const Movie& movie, const bitset<24>& hours, const vector<int>& num_watched, const vector<int>& category_limit) {
    if (num_watched[movie.category] >= category_limit[movie.category]) { return false; }
    for (int hour = movie.start_time; hour < movie.end_time; hour++) {
        if (hours[hour]) {
            return false;
        }
    }
    if (hours[movie.start_time]) { return false; }
    return true;
}

int main() {
    srand((unsigned) time(NULL));
    int num_movies, num_categories;
    cin >> num_movies >> num_categories;

    // Read the category limits into a vector
    vector<int> category_limit(num_categories + 1);
    for (int i = 1; i <= num_categories; i++) {
        cin >> category_limit[i];
    }

    // Read the movie information into a vector
    vector<Movie> unwatched_movies(num_movies);
    for (int i = 0; i < num_movies; i++) {
        cin >> unwatched_movies[i].start_time >> unwatched_movies[i].end_time >> unwatched_movies[i].category;
        if (unwatched_movies[i].end_time < unwatched_movies[i].start_time) {
            unwatched_movies[i].end_time = 24;
        }
    }

    vector<Movie> watched_movies(num_movies);
    vector<Movie> lost_movies(num_movies);

    // Sort the movies by end time
    sort(unwatched_movies.begin(), unwatched_movies.end(), [](const Movie& a, const Movie& b) {
        if (a.end_time == b.end_time) { return a.start_time < b.start_time; }
        return a.end_time < b.end_time;
    });

    // Define a bitset to represent the availability of each hour of the day
    bitset<24> hours;

    // Keep track of how many movies of each category have been watched
    vector<int> num_watched(num_categories + 1, 0);

    // Keep track of the indices of the watched movies
    vector<int> watched;

    // Iterate over the movies and check if each movie can be watched
    while (!unwatched_movies.empty()) {
        bool end = false;
        Movie movie;
        int index;
        int random_num = rand() % 4;
        if (random_num != 0) {
            index = 0;
            movie = unwatched_movies.front();
        } else {
            index = rand() % unwatched_movies.size();
            movie = unwatched_movies[index];

            while(!check_can_watch(movie, hours, num_watched, category_limit)) {
                lost_movies.push_back(movie);
                unwatched_movies.erase(unwatched_movies.begin() + index);

                if (unwatched_movies.empty()) { end = true; break; }

                index = rand() % unwatched_movies.size();
                movie = unwatched_movies[index];
            }
        }

        if (end) { break; }
        
        if (check_can_watch(movie, hours, num_watched, category_limit)) {
            hours[movie.start_time] = true;
            for (int hour = movie.start_time; hour < movie.end_time; hour++) {
                hours[hour] = true;
            }
            num_watched[movie.category]++;
            watched_movies.push_back(movie);
            unwatched_movies.erase(unwatched_movies.begin()+index);
            cout << movie.start_time << " " << movie.end_time << " " << movie.category << endl;
        } else {
            lost_movies.push_back(movie);
            unwatched_movies.erase(unwatched_movies.begin() + index);
        }
    }

    // Output the number of watched movies
    // cout << watched.size() << endl;

    return 0;
}

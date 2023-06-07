#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

struct Movie {
    int start_time;
    int end_time;
    int category;
};

// function to check if two movies overlap
bool isOverlap(const Movie &a, const Movie &b) {
    return a.start_time < b.end_time && b.start_time < a.end_time;
}

// function to check if a list of movies is valid
bool isValid(const vector<Movie> &movies, const vector<int> &category_limit) {
    vector<int> category_count(category_limit.size(), 0);
    for (size_t i = 0; i < movies.size(); i++) {
        category_count[movies[i].category]++;
        if (category_count[movies[i].category] > category_limit[movies[i].category])
            return false;
        for (size_t j = i + 1; j < movies.size(); j++)
            if (isOverlap(movies[i], movies[j]))
                return false;
    }
    return true;
}

// recursive function to generate all possible combinations
void exhaustiveSearch(vector<Movie> &movies, vector<int> &category_limit,
                     vector<Movie> &current, vector<Movie> &best, int i) {
    if (i == movies.size()) {
        if (current.size() > best.size())
            best = current;
        return;
    }
    exhaustiveSearch(movies, category_limit, current, best, i + 1);
    current.push_back(movies[i]);
    if (isValid(current, category_limit))
        exhaustiveSearch(movies, category_limit, current, best, i + 1);
    current.pop_back();
}

int main() {
    auto startTime = chrono::steady_clock::now();
    int num_movies, num_categories;
    cin >> num_movies >> num_categories;

    vector<int> category_limit(num_categories + 1);
    for (int i = 1; i <= num_categories; i++) {
        cin >> category_limit[i];
    }

    vector<Movie> movies(num_movies);
    for (int i = 0; i < num_movies; i++) {
        cin >> movies[i].start_time >> movies[i].end_time >> movies[i].category;
        if (movies[i].end_time < movies[i].start_time) {
            movies[i].end_time = 24;
        }

        if (movies[i].end_time == movies[i].start_time) {
            movies[i].end_time += 1;
        }
    }

    vector<Movie> current;
    vector<Movie> best;
    exhaustiveSearch(movies, category_limit, current, best, 0);

    for (const Movie &movie : best) {
        cout << movie.start_time << " " << movie.end_time << " " << movie.category << "\n";
    }

    cout << "\nNúmero de filmes: " << best.size() << "\n";
    auto endTime = chrono::steady_clock::now();
    double duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    cout << fixed;
    cout << "Time elapsed during the brute-force algorithm (in microseconds): " << duration << endl;
    cout << scientific;

    return 0;
}

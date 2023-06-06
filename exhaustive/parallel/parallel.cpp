#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
using namespace std;

struct Movie {
    int start_time;
    int end_time;
    int category;
};

bool isOverlap(const Movie &a, const Movie &b) {
    return a.start_time < b.end_time && b.start_time < a.end_time;
}

bool isValid(const vector<Movie> &schedule, const vector<int> &category_limit) {
    vector<int> category_count(category_limit.size(), 0);
    for (size_t i = 0; i < schedule.size(); i++) {
        category_count[schedule[i].category]++;
        if (category_count[schedule[i].category] > category_limit[schedule[i].category])
            return false;
        for (size_t j = i + 1; j < schedule.size(); j++)
            if (isOverlap(schedule[i], schedule[j]))
                return false;
    }
    return true;
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

    vector<Movie> best_schedule;

    #pragma omp parallel for
    for (int bit = 0; bit < (1 << num_movies); bit++) {
        vector<Movie> current_schedule;
        for (int i = 0; i < num_movies; i++) {
            if (bit & (1 << i)) {
                current_schedule.push_back(movies[i]);
            }
        }

        if (isValid(current_schedule, category_limit)) {
            #pragma omp critical
            {
                if (current_schedule.size() > best_schedule.size()) {
                    best_schedule = current_schedule;
                }
            }
        }
    }

    for (const Movie &movie : best_schedule) {
        cout << movie.start_time << " " << movie.end_time << " " << movie.category << "\n";
    }

    cout << "\nNúmero de filmes: " << best_schedule.size() << "\n";
    auto endTime = chrono::steady_clock::now();
    double duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    cout << fixed;
    cout << "Time elapsed during the brute-force algorithm (in microseconds): " << duration << endl;
    cout << scientific;

    return 0;
}

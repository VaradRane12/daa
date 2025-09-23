#include <iostream> 
#include <vector> 
#include <string> 
using namespace std; 
 
struct Movie { 
    string title; 
    float rating; 
    int release_year; 
    int popularity; 
 
    void display() const { 
        cout << title << " | Rating: " << rating 
             << " | Year: " << release_year 
             << " | Popularity: " << popularity << endl; 
    } 
}; 
 
// QuickSort 
int partition(vector<Movie> &movies, int low, int high, bool (*compare)(const Movie &, const Movie 
&)) { 
    Movie pivot = movies[high]; 
    int i = low - 1; 
    for (int j = low; j < high; j++) { 
        if (compare(movies[j], pivot)) { 
            i++; 
            swap(movies[i], movies[j]); 
        } 
    } 
    swap(movies[i + 1], movies[high]); 
    return i + 1; 
} 
 
void quickSort(vector<Movie> &movies, int low, int high, bool (*compare)(const Movie &, const 
Movie &)) { 
    if (low < high) { 
        int pi = partition(movies, low, high, compare); 
        quickSort(movies, low, pi - 1, compare); 
        quickSort(movies, pi + 1, high, compare); 
    } 
} 
 
// Comparators 
bool compareByRating(const Movie &a, const Movie &b) { 
    return a.rating < b.rating; 
} 
bool compareByYear(const Movie &a, const Movie &b) { 
    return a.release_year < b.release_year; 
} 
bool compareByPopularity(const Movie &a, const Movie &b) { 
    return a.popularity < b.popularity; 
} 
 
int main() { 
    vector<Movie> movies = { 
        {"Inception", 8.8, 2010, 900000}, 
        {"The Matrix", 8.7, 1999, 850000}, 
        {"Interstellar", 8.6, 2014, 870000}, 
        {"The Godfather", 9.2, 1972, 1200000}, 
        {"Avengers: Endgame", 8.4, 2019, 1100000} 
    }; 
 
    cout << "Sort movies by (rating/year/popularity): "; 
    string sort_by; 
    cin >> sort_by; 
 
    bool (*compare)(const Movie &, const Movie &); 
    if (sort_by == "rating") 
        compare = compareByRating; 
    else if (sort_by == "year") 
        compare = compareByYear; 
    else if (sort_by == "popularity") 
        compare = compareByPopularity; 
    else { 
        cout << "Invalid choice. Defaulting to rating.\n"; 
        compare = compareByRating; 
    } 
 
    quickSort(movies, 0, movies.size() - 1, compare); 
 
    cout << "\nSorted movies:\n"; 
    for (const auto &m : movies) { 
        m.display(); 
    } 
 
    return 0; 
} 

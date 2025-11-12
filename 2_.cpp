#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

struct Movie {
    string title;
    float rating;
    int release_year;
    int popularity;

    void display() const {
        cout << title << " | Rating: " << rating
             << " | Year: " << release_year
             << " | Popularity: " << popularity << "\n";
    }
};

bool compareByRating(const Movie &a, const Movie &b) {
    return a.rating < b.rating;
}
bool compareByYear(const Movie &a, const Movie &b) {
    return a.release_year < b.release_year;
}
bool compareByPopularity(const Movie &a, const Movie &b) {
    return a.popularity < b.popularity;
}

void quickSort(vector<Movie> &a, int l, int r, bool (*cmp)(const Movie&,const Movie&)) {
    if(l >= r) return;
    Movie pivot = a[r];
    int i = l;
    for(int j = l; j < r; j++) {
        if(cmp(a[j], pivot)) {
            swap(a[i], a[j]);
            i++;
        }
    }
    swap(a[i], a[r]);
    quickSort(a, l, i-1, cmp);
    quickSort(a, i+1, r, cmp);
}

vector<Movie> loadMovies(const string &file) {
    vector<Movie> v;
    ifstream f(file);
    if(!f) return v;

    string line;
    getline(f, line); // skip header
    while(getline(f, line)) {
        stringstream ss(line);
        Movie m;
        string r, y, p;
        getline(ss, m.title, ',');
        getline(ss, r, ',');
        getline(ss, y, ',');
        getline(ss, p, ',');
        if(m.title.empty()) continue;
        m.rating = stof(r);
        m.release_year = stoi(y);
        m.popularity = stoi(p);
        v.push_back(m);
    }
    return v;
}

int main() {
    vector<Movie> movies = loadMovies("movies.csv");
    if(movies.empty()) {
        cout << "No movies loaded\n";
        return 0;
    }

    cout << "Sort by (rating/year/popularity): ";
    string key;
    cin >> key;

    bool (*cmp)(const Movie&,const Movie&) = compareByRating;
    if(key=="year") cmp = compareByYear;
    else if(key=="popularity") cmp = compareByPopularity;

    auto st = high_resolution_clock::now();
    quickSort(movies, 0, movies.size()-1, cmp);
    auto et = high_resolution_clock::now();
    double t = duration<double>(et-st).count();

    cout << "\nTop 10:\n";
    for(int i = movies.size()-1; i >= max(0,(int)movies.size()-10); i--)
        movies[i].display();

    cout << "\nTime: " << t << " s\n";
    return 0;
}

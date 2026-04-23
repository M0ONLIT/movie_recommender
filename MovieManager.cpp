#include "MovieManager.h"
#include <iostream>
#include <algorithm>

void MovieManager::addMovie(int id, const std::string& title, const std::string& genre, int year) {
    movies.push_back(Movie(id, title, genre, year));
}

void MovieManager::searchByTitle(const std::string& title) const {
    bool found = false;
    for (const auto& m : movies) {
        if (m.getTitle().find(title) != std::string::npos) {
            std::cout << m << std::endl;
            found = true;
        }
    }
    if (!found) std::cout << "검색 결과가 없습니다.\n";
}

void MovieManager::printAllMovies() const {
    for (const auto& m : movies) std::cout << m << std::endl;
}

void MovieManager::printSortedMovies() const {
    if (movies.empty()) return;
    std::vector<Movie> sortedMovies = movies;
    std::sort(sortedMovies.begin(), sortedMovies.end());
    std::reverse(sortedMovies.begin(), sortedMovies.end());
    for (const auto& m : sortedMovies) std::cout << m << std::endl;
}

Movie* MovieManager::findMovieById(int id) {
    for (auto& m : movies) {
        if (m.getId() == id) return &m;
    }
    return nullptr;
}
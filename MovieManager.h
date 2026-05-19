#pragma once
#include <vector>
#include <string>
#include "movie.h"

class MovieManager {
private:
    std::vector<Movie> movies;

public:
    // 인자로 데이터를 받아서 저장만 합니다.
    void addMovie(int id, const std::string& title, const std::string& genre, int year);
    void searchByTitle(const std::string& title) const;
    void printAllMovies() const;
    void printSortedMovies() const;

    //9주차
    void loadMovies(const std::string& filename);
    void saveMovies(const std::string& filename) const;

    Movie* findMovieById(int id);
};
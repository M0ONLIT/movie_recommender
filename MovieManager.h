#pragma once
#include <vector>
#include <string>
#include "movie.h"
#include "BaseManager.h"

class MovieManager : public BaseManager {
private:
    std::vector<Movie> movies;

public:
    void addMovie(int id, const std::string& title, const std::string& genre, int year);
    void searchByTitle(const std::string& title) const;
    void printAllMovies() const;
    void printSortedMovies() const;

    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;

    Movie* findMovieById(int id);
    std::vector<std::string> getGenreList() const;
};
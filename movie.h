#pragma once
#include <string>
#include <iostream> // ostream 사용을 위해 추가

class Movie {
private:
    int         id;
    std::string title;
    std::string genre;
    int         releaseYear;
    double      totalRating; 
    int         ratingCount; 

public:
    Movie();
    Movie(int id, const std::string& title, const std::string& genre, int year);

    int         getId()           const;
    std::string getTitle()        const;
    std::string getGenre()        const;
    int         getReleaseYear()  const;
    double      getAverageRating() const;
    int         getRatingCount()  const;

    void        setReleaseYear(int year); 

    void addRating(double r);

    bool operator<(const Movie& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Movie& movie);
};
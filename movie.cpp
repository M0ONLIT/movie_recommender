#include "movie.h"

// 기본 생성자
Movie::Movie()
    : id(0), releaseYear(0), totalRating(0.0), ratingCount(0) {}

Movie::Movie(int id, const std::string& title, const std::string& genre, int year)
    : id(id), title(title), genre(genre), totalRating(0.0), ratingCount(0) {
    setReleaseYear(year);
}

int         Movie::getId()           const { return id; }
std::string Movie::getTitle()        const { return title; }
std::string Movie::getGenre()        const { return genre; }
int         Movie::getReleaseYear() const { return releaseYear; }
int         Movie::getRatingCount() const { return ratingCount; }

void Movie::setReleaseYear(int year) {
    if (year < 1888 || year > 2100) {
        std::cout << "유효하지 않은 연도입니다: " << year << " -> 기본값 0으로 설정합니다" << std::endl;
        this->releaseYear = 0;
    } else {
        this->releaseYear = year;
    }
}

double Movie::getAverageRating() const {
    if (ratingCount == 0) return 0.0;
    return totalRating / ratingCount;
}

void Movie::addRating(double r) {
    if (r < 0.0 || r > 5.0) return;
    totalRating += r;
    ratingCount++;
}

bool Movie::operator<(const Movie& other) const {
    return this->getAverageRating() < other.getAverageRating();
}

std::ostream& operator<<(std::ostream& os, const Movie& movie) {
    os << movie.id << ". " << movie.title
       << " (" << movie.releaseYear << ")"
       << "  평점: " << movie.getAverageRating()
       << " (" << movie.ratingCount << "건)";
    return os;
}
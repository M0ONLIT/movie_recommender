#include "movie.h"

namespace {

constexpr int kDefaultId = 0;
constexpr int kDefaultReleaseYear = 0;
constexpr double kDefaultTotalRating = 0.0;
constexpr int kDefaultRatingCount = 0;

constexpr int kMinReleaseYear = 1888;
constexpr int kMaxReleaseYear = 2100;

constexpr double kMinRatingScore = 0.0;
constexpr double kMaxRatingScore = 5.0;

}  // namespace

Movie::Movie()
    : id(kDefaultId),
      releaseYear(kDefaultReleaseYear),
      totalRating(kDefaultTotalRating),
      ratingCount(kDefaultRatingCount) {}

Movie::Movie(int id, const std::string& title, const std::string& genre, int year)
    : id(id),
      title(title),
      genre(genre),
      totalRating(kDefaultTotalRating),
      ratingCount(kDefaultRatingCount) {
    setReleaseYear(year);
}

int         Movie::getId()           const { return id; }
std::string Movie::getTitle()        const { return title; }
std::string Movie::getGenre()        const { return genre; }
int         Movie::getReleaseYear() const { return releaseYear; }
int         Movie::getRatingCount() const { return ratingCount; }

void Movie::setReleaseYear(int year) {
    if (year < kMinReleaseYear || year > kMaxReleaseYear) {
        std::cout << "유효하지 않은 연도입니다: " << year
                  << " -> 기본값 " << kDefaultReleaseYear << "으로 설정합니다"
                  << std::endl;
        this->releaseYear = kDefaultReleaseYear;
    } else {
        this->releaseYear = year;
    }
}

double Movie::getAverageRating() const {
    if (ratingCount == kDefaultRatingCount) {
        return kDefaultTotalRating;
    }
    return totalRating / ratingCount;
}

void Movie::addRating(double r) {
    if (r < kMinRatingScore || r > kMaxRatingScore) {
        return;
    }
    totalRating += r;
    ratingCount++;
}

bool Movie::operator<(const Movie& other) const {
    if (this->getAverageRating() == other.getAverageRating()) {
        return this->getId() < other.getId();
    }
    return this->getAverageRating() > other.getAverageRating();
}

std::ostream& operator<<(std::ostream& os, const Movie& movie) {
    os << movie.id << ". " << movie.title
       << " (" << movie.releaseYear << ")"
       << "  평점: " << movie.getAverageRating()
       << " (" << movie.ratingCount << "건)";
    return os;
}

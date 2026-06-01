#include "rating.h"

namespace {

constexpr int kDefaultUserId = 0;
constexpr int kDefaultMovieId = 0;
constexpr double kDefaultScore = 0.0;

constexpr double kMinRatingScore = 0.0;
constexpr double kMaxRatingScore = 5.0;

}  // namespace

Rating::Rating()
    : userId(kDefaultUserId), movieId(kDefaultMovieId), score(kDefaultScore) {}

Rating::Rating(int userId, int movieId, double score)
    : userId(userId), movieId(movieId) {
    if (score < kMinRatingScore || score > kMaxRatingScore) {
        this->score = kDefaultScore;
    } else {
        this->score = score;
    }
}

int Rating::getUserId() const { return userId; }
int Rating::getMovieId() const { return movieId; }
double Rating::getScore() const { return score; }

std::ostream& operator<<(std::ostream& os, const Rating& rating) {
    os << "[평가 정보] 사용자ID: " << rating.userId
       << " | 영화ID: " << rating.movieId
       << " | 평점: " << rating.score << "/" << kMaxRatingScore;
    return os;
}

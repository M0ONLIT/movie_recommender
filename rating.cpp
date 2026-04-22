#include "rating.h"

Rating::Rating() : userId(0), movieId(0), score(0.0) {}

Rating::Rating(int userId, int movieId, double score) 
    : userId(userId), movieId(movieId) {
    
    if (score < 0.0 || score > 5.0) {
        this->score = 0.0;
    } else {
        this->score = score;
    }
}

// Getter 함수들
int Rating::getUserId() const { return userId; }
int Rating::getMovieId() const { return movieId; }
double Rating::getScore() const { return score; }

std::ostream& operator<<(std::ostream& os, const Rating& rating) {
    os << "[평가 정보] 사용자ID: " << rating.userId 
       << " | 영화ID: " << rating.movieId 
       << " | 평점: " << rating.score << "/5.0" << std::endl;
    return os;
}
#include "rating.h"
#include <iostream>

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

// 정보 출력 함수
void Rating::display() {
    std::cout << "[평가 정보] 사용자ID: " << userId 
              << " | 영화ID: " << movieId 
              << " | 평점: " << score << "/5.0" << std::endl;
}
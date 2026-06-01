#pragma once
#include <iostream>

class Rating {
private:
    int     userId;    // 평가한 사용자
    int     movieId;   // 평가 대상 영화
    double  score;

public:
    Rating();
    Rating(int userId, int movieId, double score);
    int getUserId() const;
    int getMovieId() const;
    double getScore() const;
    friend std::ostream& operator<<(std::ostream& os, const Rating& rating);
};
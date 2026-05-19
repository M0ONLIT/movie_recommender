#pragma once
#include <vector>
#include "rating.h"
#include "MovieManager.h"
#include "UserManager.h"

class RatingManager {
private:
    std::vector<Rating> ratings;

public:
    // MovieManager를 인자로 받아 영화 평점까지 업데이트합니다.
    void addRating(int uId, int mId, double score, MovieManager& mm, UserManager& um);
    void printRatingsByMovie(int mId) const;

    void loadRatings(const std::string& filename, MovieManager& mm, UserManager& um);
    void saveRatings(const std::string& filename) const;
};
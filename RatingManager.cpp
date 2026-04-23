#include "RatingManager.h"
#include <iostream>

void RatingManager::addRating(int uId, int mId, double score, MovieManager& mm, UserManager& um) {
    ratings.push_back(Rating(uId, mId, score));

    Movie* target = mm.findMovieById(mId);
    User* target2 = um.findUserById(uId);
    if (target && target2) {
        target->addRating(score);
        std::cout << "평점이 성공적으로 반영되었습니다.\n";
    } else if(target2) {
        std::cout << "오류: 해당 ID의 영화가 존재하지 않습니다.\n";
    } else {
        std::cout << "오류: 해당 ID의 유저가 존재하지 않습니다.\n";
    }
}

void RatingManager::printRatingsByMovie(int mId) const {
    bool found = false;
    for (const auto& r : ratings) {
        if (r.getMovieId() == mId) {
            found = true;
        }
    }
    if (!found) std::cout << "해당 영화에 대한 평점 기록이 없습니다.\n";
}
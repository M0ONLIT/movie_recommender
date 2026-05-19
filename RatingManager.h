#pragma once
#include <vector>
#include "rating.h"
#include "MovieManager.h"
#include "UserManager.h"
#include "BaseManager.h"

class RatingManager : public BaseManager {
private:
    std::vector<Rating> ratings;
    MovieManager& mm;
    UserManager& um;

public:
    RatingManager(MovieManager& mm, UserManager& um);

    void addRating(int uId, int mId, double score);
    void printRatingsByMovie(int mId) const;
    std::vector<Rating> findByUser(int uId) const;
    std::vector<int> getAllUserIds() const;

    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;
};

#pragma once

#include <vector>
#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "movie.h"

class Recommender {
private:
    MovieManager& mm;
    UserManager& um;
    RatingManager& rm;

public:
    Recommender(MovieManager& mm, UserManager& um, RatingManager& rm);

    std::vector<Movie> recommend(int userId) const;
    double distance(int userId1, int userId2) const;
};

#pragma once

#include <map>
#include <string>
#include <unordered_set>
#include <utility>
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

    std::unordered_set<int> getSeenMovies(int userId) const;
    bool hasReachableUser(int userId,
                          const std::map<int, double>& userDist) const;
    bool matchesGenre(int movieId, const std::string& genre) const;
    std::map<int, double> computeMovieScores(
        int userId,
        const std::unordered_set<int>& seenByTarget,
        const std::map<int, double>& userDist,
        const std::string& genre) const;
    std::vector<std::pair<double, int>> buildSortedCandidates(
        const std::map<int, double>& movieScores) const;
    std::vector<Movie> selectTopMovies(
        const std::vector<std::pair<double, int>>& candidates,
        const std::string& genre) const;

public:
    Recommender(MovieManager& mm, UserManager& um, RatingManager& rm);

    std::vector<Movie> recommend(int userId, const std::string& genre = "") const;
    double distance(int userId1, int userId2) const;
};

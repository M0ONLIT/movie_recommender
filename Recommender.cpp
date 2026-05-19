#include "Recommender.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
#include <queue>
#include <unordered_set>
#include <utility>
#include <vector>

namespace {

using UserGraph = std::map<int, std::vector<std::pair<int, double>>>;

UserGraph buildUserGraph(const std::vector<int>& userIds,
                         const Recommender& recommender) {
    const double INF = std::numeric_limits<double>::max();
    UserGraph graph;
    for (int userId : userIds) {
        graph[userId] = {};
    }

    for (std::size_t i = 0; i < userIds.size(); ++i) {
        for (std::size_t j = i + 1; j < userIds.size(); ++j) {
            const double edgeWeight =
                recommender.distance(userIds[i], userIds[j]);
            if (edgeWeight >= INF) {
                continue;
            }
            graph[userIds[i]].push_back({userIds[j], edgeWeight});
            graph[userIds[j]].push_back({userIds[i], edgeWeight});
        }
    }
    return graph;
}

std::map<int, double> dijkstra(const UserGraph& graph, int source) {
    const double INF = std::numeric_limits<double>::max();
    std::map<int, double> dist;
    for (const auto& entry : graph) {
        dist[entry.first] = INF;
    }
    if (dist.find(source) == dist.end()) {
        return dist;
    }

    dist[source] = 0.0;
    using Node = std::pair<double, int>;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;
    pq.push({0.0, source});

    while (!pq.empty()) {
        const auto [currentDist, u] = pq.top();
        pq.pop();
        if (currentDist > dist[u]) {
            continue;
        }

        for (const auto& [v, weight] : graph.at(u)) {
            const double nextDist = dist[u] + weight;
            if (nextDist < dist[v]) {
                dist[v] = nextDist;
                pq.push({nextDist, v});
            }
        }
    }
    return dist;
}

int reviewCountForMovie(int movieId, const RatingManager& rm) {
    int count = 0;
    for (int userId : rm.getAllUserIds()) {
        for (const Rating& rating : rm.findByUser(userId)) {
            if (rating.getMovieId() == movieId) {
                ++count;
            }
        }
    }
    return count;
}

}  // namespace

Recommender::Recommender(MovieManager& mm, UserManager& um, RatingManager& rm)
    : mm(mm), um(um), rm(rm) {}

double Recommender::distance(int userId1, int userId2) const {
    const std::vector<Rating> user1 = rm.findByUser(userId1);
    const std::vector<Rating> user2 = rm.findByUser(userId2);

    int commonCount = 0;
    double scoreDiffSum = 0.0;

    for (const Rating& r1 : user1) {
        for (const Rating& r2 : user2) {
            if (r1.getMovieId() == r2.getMovieId()) {
                commonCount++;
                scoreDiffSum += std::abs(r1.getScore() - r2.getScore());
            }
        }
    }

    if (commonCount == 0) {
        return std::numeric_limits<double>::max();
    }

    return scoreDiffSum / commonCount;
}

std::vector<Movie> Recommender::recommend(int userId) const {
    const std::vector<Rating> targetRatings = rm.findByUser(userId);
    if (targetRatings.empty()) {
        return {};
    }

    std::unordered_set<int> seenByTarget;
    for (const Rating& rating : targetRatings) {
        seenByTarget.insert(rating.getMovieId());
    }

    const std::vector<int> userIds = rm.getAllUserIds();
    const UserGraph graph = buildUserGraph(userIds, *this);
    const std::map<int, double> userDist = dijkstra(graph, userId);
    const double INF = std::numeric_limits<double>::max();

    bool hasReachableUser = false;
    for (const auto& entry : userDist) {
        if (entry.first != userId && entry.second < INF) {
            hasReachableUser = true;
            break;
        }
    }
    if (!hasReachableUser) {
        return {};
    }

    std::map<int, double> movieScores;

    for (int otherId : userIds) {
        if (otherId == userId) {
            continue;
        }

        const auto distIt = userDist.find(otherId);
        if (distIt == userDist.end() || distIt->second >= INF) {
            continue;
        }

        const double userWeight = 1.0 / (distIt->second + 1.0);
        for (const Rating& rating : rm.findByUser(otherId)) {
            const int movieId = rating.getMovieId();
            if (seenByTarget.count(movieId) > 0) {
                continue;
            }
            movieScores[movieId] += userWeight * rating.getScore();
        }
    }

    std::vector<std::pair<double, int>> candidates;
    for (const auto& entry : movieScores) {
        const int movieId = entry.first;
        const int reviewCount = reviewCountForMovie(movieId, rm);
        const double reviewFactor =
            2.0 - std::exp(1.0 - static_cast<double>(reviewCount));
        candidates.emplace_back(entry.second * reviewFactor, movieId);
    }

    std::sort(candidates.begin(), candidates.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    std::vector<Movie> result;
    const std::size_t limit = std::min<std::size_t>(3, candidates.size());
    result.reserve(limit);

    for (std::size_t i = 0; i < limit; ++i) {
        Movie* movie = mm.findMovieById(candidates[i].second);
        if (movie != nullptr) {
            result.push_back(*movie);
        }
    }

    return result;
}

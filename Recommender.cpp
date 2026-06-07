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

std::unordered_set<int> Recommender::getSeenMovies(int userId) const {
    std::unordered_set<int> seenByTarget;
    for (const Rating& rating : rm.findByUser(userId)) {
        seenByTarget.insert(rating.getMovieId());
    }
    return seenByTarget;
}

bool Recommender::hasReachableUser(
    int userId, const std::map<int, double>& userDist) const {
    const double INF = std::numeric_limits<double>::max();
    for (const auto& entry : userDist) {
        if (entry.first != userId && entry.second < INF) {
            return true;
        }
    }
    return false;
}

bool Recommender::matchesGenre(int movieId, const std::string& genre) const {
    if (genre.empty()) {
        return true;
    }
    const Movie* movie = mm.findMovieById(movieId);
    return movie != nullptr && movie->getGenre() == genre;
}

std::map<int, double> Recommender::computeMovieScores(
    int userId,
    const std::unordered_set<int>& seenByTarget,
    const std::map<int, double>& userDist,
    const std::string& genre) const {
    const double INF = std::numeric_limits<double>::max();
    std::map<int, double> movieScores;

    for (int otherId : rm.getAllUserIds()) {
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
            if (!matchesGenre(movieId, genre)) {
                continue;
            }
            movieScores[movieId] += userWeight * rating.getScore();
        }
    }

    return movieScores;
}

std::vector<std::pair<double, int>> Recommender::buildSortedCandidates(
    const std::map<int, double>& movieScores) const {
    std::vector<std::pair<double, int>> candidates;
    candidates.reserve(movieScores.size());

    for (const auto& entry : movieScores) {
        const int movieId = entry.first;
        const int reviewCount = reviewCountForMovie(movieId, rm);
        const double reviewFactor =
            2.0 - std::exp(1.0 - static_cast<double>(reviewCount));
        candidates.emplace_back(entry.second * reviewFactor, movieId);
    }

    std::sort(candidates.begin(), candidates.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });
    return candidates;
}

std::vector<Movie> Recommender::selectTopMovies(
    const std::vector<std::pair<double, int>>& candidates,
    const std::string& genre) const {
    std::vector<Movie> result;
    result.reserve(std::min<std::size_t>(3, candidates.size()));

    for (const auto& candidate : candidates) {
        if (result.size() >= 3) {
            break;
        }
        if (!matchesGenre(candidate.second, genre)) {
            continue;
        }
        Movie* movie = mm.findMovieById(candidate.second);
        if (movie != nullptr) {
            result.push_back(*movie);
        }
    }

    return result;
}

/*
 * Recommender::recommend(userId) — 영화 추천 (최대 3편, 0편일 수 있음)
 *
 * [전제]
 * - 유저 = 그래프의 노드. 두 유저가 같은 영화를 본 적 있으면 간선이 있고,
 *   간선 가중치 = distance(u, v) (공통 영화 평점 차의 평균, 작을수록 비슷함).
 * - 서로 다른 연결 요소(분리된 집합)는 합치지 않음.
 *
 * [1] 대상 유저(userId)가 본 영화가 하나도 없으면 → 빈 벡터 반환.
 *
 * [2] seenByTarget: 대상 유저가 이미 평가한 movieId 집합.
 *     이후 후보에서 제외 (아직 보지 않은 영화만 추천).
 *
 * [3] 유저 그래프 구성 (buildUserGraph)
 *     - 모든 유저 쌍에 대해 distance() 호출.
 *     - 공통 영화가 없으면 간선 없음 (INF).
 *     - 있으면 양방향 간선, 가중치 = distance 값.
 *
 * [4] 다익스트라 (dijkstra)로 userId에서 각 유저까지 최단 거리 userDist 계산.
 *     - 직접 겹치는 영화가 없어도 중간 유저를 거치면 거리 정의 가능.
 *     - 도달 불가(INF)인 유저는 추천 가중치에 쓰지 않음.
 *
 * [5] userDist 기준으로 다른 유저 중 하나라도 도달 가능하지 않으면 → 빈 벡터.
 *
 * [6] 영화별 점수 movieScores (대상이 아직 안 본 영화만)
 *     - 도달 가능한 다른 유저 i, i가 준 영화 j에 대해:
 *       movieScores[j] += (1 / (userDist[i] + 1)) * (i의 j 평점)
 *     - 가까운 유저(거리 작음)일수록 가중치 1/(거리+1)이 커져 더 많이 반영.
 *
 * [7] 최종 점수 및 정렬
 *     - 영화 j의 점수 = movieScores[j] * (2 - e^(1 - review_count(j)))
 *     - review_count(j): 시스템 전체에서 j에 달린 리뷰 개수.
 *     - 점수 내림차순 정렬 후 상위 최대 3개 movieId를 MovieManager에서 찾아 반환.
 */
std::vector<Movie> Recommender::recommend(int userId, const std::string& genre) const {
    const std::unordered_set<int> seenByTarget = getSeenMovies(userId);
    if (seenByTarget.empty()) {
        return {};
    }

    const std::vector<int> userIds = rm.getAllUserIds();
    const UserGraph graph = buildUserGraph(userIds, *this);
    const std::map<int, double> userDist = dijkstra(graph, userId);
    if (!hasReachableUser(userId, userDist)) {
        return {};
    }

    const std::map<int, double> movieScores =
        computeMovieScores(userId, seenByTarget, userDist, genre);
    const std::vector<std::pair<double, int>> candidates =
        buildSortedCandidates(movieScores);

    return selectTopMovies(candidates, genre);
}

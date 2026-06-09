#include "RatingManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

RatingManager::RatingManager(MovieManager& mm, UserManager& um): mm(mm), um(um) {}

void RatingManager::addRating(int uId, int mId, double score) {
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
            std::cout << r << std::endl;
            found = true;
        }
    }
    if (!found) std::cout << "해당 영화에 대한 평점 기록이 없습니다.\n";
}

std::vector<Rating> RatingManager::findByUser(int uId) const {
    std::vector<Rating> result;
    for (const auto& r : ratings) {
        if (r.getUserId() == uId) {
            result.push_back(r);
        }
    }
    return result;
}

std::vector<int> RatingManager::getAllUserIds() const {
    std::vector<int> ids;
    for (const auto& r : ratings) {
        int uid = r.getUserId();
        if (std::find(ids.begin(), ids.end(), uid) == ids.end()) {
            ids.push_back(uid);
        }
    }
    return ids;
}

void RatingManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: " << filename << " 파일을 열 수 없습니다." << std::endl;
        return;
    }
    std::string line;
    while (getline(file, line)) {
        try {
            std::stringstream ss(line);
            std::string token;
            std::string uId_str, mId_str, score_str;

            getline(ss, uId_str, ',');
            getline(ss, mId_str, ',');
            getline(ss, score_str, ',');

            if (uId_str.empty() || mId_str.empty() || score_str.empty()) {
                continue;
            }
            int uId = stoi(uId_str);
            int mId = stoi(mId_str);
            double score = stod(score_str);

            Movie* target = mm.findMovieById(mId);
            User* target2 = um.findUserById(uId);
            if (target && target2) {
                ratings.push_back(Rating(uId, mId, score));
                target->addRating(score);
            } else if (target2) {
                std::cout << "오류: 해당 ID의 영화가 존재하지 않아 평점 기록이 무시됩니다. ID: " << mId << "\n";
            } else {
                std::cout << "오류: 해당 ID의 유저가 존재하지 않아 평점 기록이 무시됩니다. ID: " << uId << "\n";
            }
        } catch (...) {
            continue;
        }
    }
    file.close();
    std::cout << filename << " 로드 완료: " << ratings.size() << "건" << std::endl;
}

void RatingManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: " << filename << " 파일을 열 수 없습니다." << std::endl;
        return;
    }
    for (const auto& r : ratings) {
        file << r.getUserId() << "," << r.getMovieId() << "," << r.getScore() << std::endl;
    }
    file.close();
    std::cout << filename << " 저장 완료: " << ratings.size() << "건" << std::endl;
}
#include "MovieManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>


void MovieManager::addMovie(int id, const std::string& title, const std::string& genre, int year) {
    movies.push_back(Movie(id, title, genre, year));
}

void MovieManager::searchByTitle(const std::string& title) const {
    bool found = false;
    for (const auto& m : movies) {
        if (m.getTitle().find(title) != std::string::npos) {
            std::cout << m << std::endl;
            found = true;
        }
    }
    if (!found) std::cout << "검색 결과가 없습니다.\n";
}

void MovieManager::printAllMovies() const {
    for (const auto& m : movies) std::cout << m << std::endl;
}

void MovieManager::printSortedMovies() const {
    if (movies.empty()) return;
    std::vector<Movie> sortedMovies = movies;
    std::sort(sortedMovies.begin(), sortedMovies.end());
    for (const auto& m : sortedMovies) std::cout << m << std::endl;
}

// 1. 영화 데이터 불러오기
void MovieManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: " << filename << " 파일을 열 수 없습니다." << std::endl;
        return;
    }
    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::string id_str, title, genre, year_str;
        
        getline(ss, id_str, ',');
        getline(ss, title, ',');
        getline(ss, genre, ',');
        getline(ss, year_str, ',');

        if (id_str.empty() || title.empty() || genre.empty() || year_str.empty()) {
            continue; 
        }
        int id = stoi(id_str);
        int year = stoi(year_str);
        addMovie(id, title, genre, year);
    }
    
    file.close();
    std::cout << filename << " 로드 완료: " << movies.size() << "건" << std::endl;
}

void MovieManager::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: " << filename << " 저장 실패" << std::endl;
        return;
    }

    for (const auto& m : movies) {
        file << m.getId() << ","
             << m.getTitle() << ","
             << m.getGenre() << ","
             << m.getReleaseYear() << std::endl;
    }

    file.close();
    std::cout << filename << " 저장 완료: " << movies.size() << "건" << std::endl;
}

Movie* MovieManager::findMovieById(int id) {
    for (auto& m : movies) {
        if (m.getId() == id) return &m;
    }
    return nullptr;
}
#include "UserManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

void UserManager::addUser(int id, const std::string& name, const std::string& email) {
    users.push_back(User(id, name, email));
}

void UserManager::printAllUsers() const {
    for (const auto& u : users) std::cout << u << std::endl;
}

User* UserManager::findUserById(int id) {
    for (auto& u : users) {
        if (u.getId() == id) return &u;
    }
    return nullptr;
}

void UserManager::loadUsers(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: " << filename << " 파일을 열 수 없습니다." << std::endl;
        return;
    }
    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::string id_str, name, email;
        
        getline(ss, id_str, ',');
        getline(ss, name, ',');
        getline(ss, email, ',');

        if (id_str.empty() || name.empty() || email.empty()) {
            continue; 
        }
        int id = stoi(id_str);
        addUser(id, name, email);
    }
    
    file.close();
    std::cout << filename << " 로드 완료: " << users.size() << "건" << std::endl;
}

void UserManager::saveUsers(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: " << filename << " 파일을 열 수 없습니다." << std::endl;
        return;
    }
    for (const auto& u : users) {
        file << u.getId() << "," << u.getName() << "," << u.getEmail() << std::endl;
    }
    std::cout << filename << " 저장 완료: " << users.size() << "건" << std::endl;
    file.close();
}
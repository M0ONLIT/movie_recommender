#include "UserManager.h"
#include <iostream>

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
#pragma once
#include <vector>
#include <string>
#include "user.h"

class UserManager {
private:
    std::vector<User> users;

public:
    void addUser(int id, const std::string& name, const std::string& email);
    void printAllUsers() const;

    void loadUsers(const std::string& filename);
    void saveUsers(const std::string& filename) const;

    User* findUserById(int id);
};
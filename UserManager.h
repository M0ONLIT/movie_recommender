#pragma once
#include <vector>
#include <string>
#include "user.h"
#include "BaseManager.h"

class UserManager : public BaseManager {
private:
    std::vector<User> users;

public:
    void addUser(int id, const std::string& name, const std::string& email);
    void printAllUsers() const;

    void loadFromFile(const std::string& filename) override;
    void saveToFile(const std::string& filename) const override;

    User* findUserById(int id);
};

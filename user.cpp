#include "user.h"
#include <iostream>

User::User() : id(0), name("Unknown"), email("None") {}

User::User(int id, const std::string &name, const std::string &email) 
    : id(id), name(name), email(email) {}

int User::getId() const { return id; }
std::string User::getName() const { return name; }
std::string User::getEmail() const { return email; }

void User::display() {
    std::cout << "사용자의 ID: " << id << '\n';
    std::cout << "사용자의 이름: " << name << '\n';
    std::cout << "사용자의 email: " << email << '\n';
}
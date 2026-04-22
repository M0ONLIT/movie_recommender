#include "user.h"

User::User() : id(0), name("Unknown"), email("None") {}

User::User(int id, const std::string &name, const std::string &email) 
    : id(id), name(name), email(email) {}

int User::getId() const { return id; }
std::string User::getName() const { return name; }
std::string User::getEmail() const { return email; }

std::ostream& operator<<(std::ostream& os, const User& user){
    os << "ID: " << user.id << 
       << " | 이름: " << user.name 
       << " | email: " << user.email << std::endl;
    return os;
}
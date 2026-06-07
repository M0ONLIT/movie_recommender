#include "user.h"

namespace {
    constexpr int kDefaultId = 0;
    const std::string kDefaultname = "Unknown";
    const std::string kDefaultEmail = "None";    
}

User::User() : id(kDefaultId), name(kDefaultname), email(kDefaultEmail) {}

User::User(int id, const std::string &name, const std::string &email) 
    : id(id), name(name), email(email) {}

int User::getId() const { return id; }
std::string User::getName() const { return name; }
std::string User::getEmail() const { return email; }

std::ostream& operator<<(std::ostream& os, const User& user){
    os << "ID: " << user.id
       << " | 이름: " << user.name
       << " | email: " << user.email;
    return os;
}
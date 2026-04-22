#include "movie.h"
#include <iostream>

// 기본 생성자
Movie::Movie()
    : id(0), releaseYear(0),
      totalRating(0.0), ratingCount(0) {}

// 4인자 생성자
Movie::Movie(int id, const std::string& title, const std::string& genre, int year)
    : id(id), title(title), genre(genre), releaseYear(0), totalRating(0.0), ratingCount(0) {
    
    setReleaseYear(year);


}

int         Movie::getId()          const { return id; }
std::string Movie::getTitle()       const { return title; }
std::string Movie::getGenre()       const { return genre; }
int         Movie::getReleaseYear() const { return releaseYear; }
int         Movie::getRatingCount() const { return ratingCount; }

void Movie::setReleaseYear(int year) const{
    if(year<1888 || year>2100){
        std::cout<<"유효하지 않은 연도입니다:"<<year<<"→기본값 0으로 설정합니다"<<std::endl;
    }
}

double Movie::getAverageRating() const {
    if (ratingCount == 0) return 0.0;  // 0 나눗셈 방어
    return totalRating / ratingCount;
}

void Movie::addRating(double r) {
    if (r < 0.0 || r > 5.0) return;   // 유효성 검사
    totalRating += r;
    ratingCount++;
}

void Movie::display() const {          // const 추가
    std::cout << id << ". " << title
              << " (" << releaseYear << ")"
              << "  평점: " << getAverageRating()
              << " (" << ratingCount << "건)"
              << std::endl;
}
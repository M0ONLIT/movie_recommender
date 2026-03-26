#include "movie.h"
#include "rating.h"
#include "user.h"
#include <iostream>
#include <vector>

int main() {
    Movie m1(1, "기생충",    "드라마", 2019);
    Movie m2(2, "인터스텔라", "SF",    2014);

    m1.addRating(4.8);
    m2.addRating(4.9);

    m1.display();
    // 1. 기생충 (2019)  평점: 4.8 (1건)
    m2.display();
    // 2. 인터스텔라 (2014)  평점: 4.9 (1건)

    m1.addRating(5.0);
    m1.addRating(6.0);   // 유효성 검사 → 무시됨

    std::cout << m1.getTitle() << ": "
              << m1.getAverageRating() << std::endl;
    // 기생충: 4.9  (4.8 + 5.0) / 2

    std::vector<Movie> movies;

    // 영화 추가 (push_back)
    movies.push_back(Movie(1, "Inception",    "Sci-Fi",   2010));
    movies.push_back(Movie(2, "Parasite",     "Thriller", 2019));
    movies.push_back(Movie(3, "Interstellar", "Sci-Fi",   2014));

    // 평점 추가
    movies[0].addRating(4.8);
    movies[0].addRating(4.5);
    movies[1].addRating(5.0);

    // 전체 출력 (range-based for, const 참조)
    for (const Movie& m : movies) {
        m.display();
    }

    std::cout << "총 " << movies.size() << "편" << std::endl;


    //user.cpp
    // 1. 기본 생성자 테스트
    User defaultUser;
    std::cout << "--- 기본 생성자 테스트 ---" << std::endl;
    defaultUser.display();
    std::cout << std::endl;

    // 2. 매개변수 생성자 테스트
    User user1(101, "MokaLatte", "moka@example.com");
    User user2(102, "O3O", "o3o@crypto.com");

    std::cout << "--- 사용자 정보 출력 ---" << std::endl;
    user1.display();
    std::cout << "-----------------------" << std::endl;
    user2.display();
    std::cout << std::endl;

    // 3. Getter 함수 개별 테스트
    std::cout << "--- Getter 테스트 ---" << std::endl;
    std::cout << "User 1의 이름만 추출: " << user1.getName() << std::endl;


    //rating
    Rating r1(101, 10, 4.5);
    Rating r2(102, 20, 3.8);

    std::cout << "--- 정상 데이터 출력 ---" << std::endl;
    r1.display();
    r2.display();
    std::cout << std::endl;

    // 2. 유효성 검사 테스트 (범위를 벗어난 평점)
    std::cout << "--- 유효성 검사 테스트 (잘못된 값 6.5 입력) ---" << std::endl;
    Rating r3(103, 30, 6.5); // 경고 메시지가 출력되어야 함
    r3.display();
    std::cout << std::endl;


}
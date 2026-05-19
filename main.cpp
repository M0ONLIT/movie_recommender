#include "movie.h"
#include "rating.h"
#include "user.h"
#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "SimilarityCalculator.h"

#include <windows.h>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
using namespace std;


template <typename T>
void getInput(T &x) {
    while (true) {
        std::cin >> x;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "잘못된 형식입니다. 다시 입력해 주세요.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }
}

void showMenu() {
    cout << "\n=== Movie Recommender ===\n\n";
    cout << "[ 영화 ]\n";
    cout << "  1. 영화 추가\n";
    cout << "  2. 제목으로 검색\n";
    cout << "  3. 전체 목록 출력\n";
    cout << "  4. 평점순 정렬 출력\n\n";
    cout << "[ 사용자 ]\n";
    cout << "  5. 사용자 추가\n";
    cout << "  6. 사용자 목록 출력\n\n";
    cout << "[ 평점 ]\n";
    cout << "  7. 평점 입력\n";
    cout << "  8. 영화별 평점 보기\n\n";
    cout << "  0. 종료\n\n";
    cout << "선택 > ";
}

/*
int main() {
    MovieManager mm;
    UserManager um;
    RatingManager rm;

    mm.loadMovies("./data/movie.csv");
    um.loadUsers("./data/user.csv");
    rm.loadRatings("./data/rating.csv", mm, um);

    int choice;
    while (true) {
        showMenu();
        getInput(choice);

        if (choice == 0) break;

        switch (choice) {
            case 1: {
                int id, year;
                string title, genre;
                cout << "ID: "; getInput(id);
                cout << "제목: "; getline(cin, title);
                cout << "장르: "; getline(cin, genre);
                cout << "개봉연도: "; getInput(year);
                mm.addMovie(id, title, genre, year);
                break;
            }
            case 2: {
                string title;
                cout << "검색할 제목: "; getline(cin, title);
                mm.searchByTitle(title);
                break;
            }
            case 3: mm.printAllMovies(); break;
            case 4: mm.printSortedMovies(); break;
            case 5: {
                int id;
                string name, email;
                cout << "ID: "; getInput(id);
                cout << "이름: "; getline(cin, name);
                cout << "이메일: "; getline(cin, email);
                um.addUser(id, name, email);
                break;
            }
            case 6: um.printAllUsers(); break;
            case 7: {
                int uId, mId;
                double score;
                cout << "사용자 ID: "; getInput(uId);
                cout << "영화 ID: "; getInput(mId);
                cout << "평점(0-5): "; getInput(score);
                rm.addRating(uId, mId, score, mm, um);
                break;
            }
            case 8: {
                int mId;
                cout << "영화 ID: "; getInput(mId);
                rm.printRatingsByMovie(mId);
                break;
            }
            default: cout << "잘못된 메뉴 선택입니다.\n";
        }
    }

    mm.saveMovies("./data/movie.csv");
    um.saveUsers("./data/user.csv");
    rm.saveRatings("./data/rating.csv");
    return 0;
}
*/

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    MovieManager mm;
    UserManager um;
    RatingManager mgr;

    mm.loadMovies("./data/movie.csv");
    um.loadUsers("./data/user.csv");
    mgr.loadRatings("./data/rating.csv", mm, um);

    int targetUserId = 1;
    std::vector<Rating> myRatings = mgr.findByUser(targetUserId);

    int bestUser = -1;
    int bestSim = -101;

    for (int otherId : mgr.getAllUserIds()) {
        if (otherId == targetUserId) continue;

        std::vector<Rating> otherRatings = mgr.findByUser(otherId);
        int sim = SimilarityCalculator::calculate(myRatings, otherRatings);

        if (sim > bestSim) {
            bestSim = sim;
            bestUser = otherId;
        }
    }

    std::cout << "User " << targetUserId
              << "와 가장 비슷한 사용자: User " << bestUser
              << " (유사도 " << bestSim << ")" << std::endl;

    return 0;
}

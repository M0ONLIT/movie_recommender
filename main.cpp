#include "movie.h"
#include "rating.h"
#include "user.h"
#include "MovieManager.h"
#include "UserManager.h"
#include "RatingManager.h"
#include "Recommender.h"

#include <windows.h>  // Windows 전용 (리눅스 채점용 비활성화)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <iomanip>
using namespace std;

enum class Operation {
    EXIT = 0,

    ADD_MOVIE = 1,
    SEARCH_MOVIE_WITH_TITLE = 2,
    PRINT_ALL_MOVIES = 3,
    PRINT_SORTED_MOVIES = 4,

    ADD_USER = 5,
    PRINT_ALL_USERS = 6,
    
    ADD_MOVIE_RATING = 7,
    CHECK_MOVIE_RATING = 8,
    RECOMMEND_MOVIES = 9,
    PRINT_AVERAGE_RATING_BY_GENRE = 10
};


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
    cout << "  8. 영화별 평점 보기\n";
    cout << "  9. 영화 추천받기\n";
    cout << " 10. 장르별 평점 보기\n\n";
    cout << "  0. 종료\n\n";
    cout << "선택 > ";
}


int main() {
    SetConsoleOutputCP(CP_UTF8); // Windows 전용
    SetConsoleCP(CP_UTF8); // Windows 전용
    // mingw32-make

    std::cout << fixed << setprecision(2);

    MovieManager mm;
    UserManager um;
    RatingManager rm(mm, um);
    Recommender rec(mm, um, rm);

    mm.loadFromFile("./data/movie.csv");
    um.loadFromFile("./data/user.csv");
    rm.loadFromFile("./data/rating.csv");

    int choice;
    while (true) { try {
        showMenu();
        getInput(choice);
        Operation op = static_cast<Operation>(choice);

        if (op == Operation::EXIT) break;

        switch (op) {
        case Operation::ADD_MOVIE: {
            int id, year;
            string title, genre;
            cout << "ID: "; getInput(id);
            cout << "제목: "; getline(cin, title);
            cout << "장르: "; getline(cin, genre);
            cout << "개봉연도: "; getInput(year);
            mm.addMovie(id, title, genre, year);
            break;
        }
        case Operation::SEARCH_MOVIE_WITH_TITLE: {
            string title;
            cout << "검색할 제목: "; getline(cin, title);
            mm.searchByTitle(title);
            break;
        }
        case Operation::PRINT_ALL_MOVIES: 
            mm.printAllMovies(); 
            break;
            
        case Operation::PRINT_SORTED_MOVIES: 
            mm.printSortedMovies(); 
            break;
            
        case Operation::ADD_USER: {
            int id;
            string name, email;
            cout << "ID: "; getInput(id);
            cout << "이름: "; getline(cin, name);
            cout << "이메일: "; getline(cin, email);
            um.addUser(id, name, email);
            break;
        }
        case Operation::PRINT_ALL_USERS: 
            um.printAllUsers(); 
            break;
            
        case Operation::ADD_MOVIE_RATING: {
            int uId, mId;
            double score;
            cout << "사용자 ID: "; getInput(uId);
            cout << "영화 ID: "; getInput(mId);
            cout << "평점(0-5): "; getInput(score);
            rm.addRating(uId, mId, score);
            break;
        }
        case Operation::CHECK_MOVIE_RATING: {
            int mId;
            cout << "영화 ID: "; getInput(mId);
            rm.printRatingsByMovie(mId);
            break;
        }
        case Operation::RECOMMEND_MOVIES: {
            int uid;
            cout << "사용자 ID: "; getInput(uid);
            if (um.findUserById(uid) == nullptr) {
                cout << "해당 ID의 사용자가 존재하지 않습니다.\n";
                break;
            }

            const vector<string> genres = mm.getGenreList();
            cout << "장르를 고르시오\n";
            cout << "0. 장르 없음\n";
            for (size_t i = 0; i < genres.size(); ++i) {
                cout << (i + 1) << ". " << genres[i] << "\n";
            }

            int genreChoice;
            cout << "선택: ";
            getInput(genreChoice);

            string genre;
            if (genreChoice == 0) {
                genre = "";
            } else if (genreChoice > 0
                    && static_cast<size_t>(genreChoice) <= genres.size()) {
                genre = genres[genreChoice - 1];
            } else {
                cout << "잘못된 선택입니다.\n";
                break;
            }

            const vector<Movie> recommended = rec.recommend(uid, genre);
            if (recommended.empty()) {
                cout << "추천할 영화가 없습니다.\n";
            } else {
                cout << "추천 영화:\n";
                for (const Movie& movie : recommended) {
                    cout << movie << endl;
                }
            }
            break;
        }
        case Operation::PRINT_AVERAGE_RATING_BY_GENRE: {
            const auto avgByGenre = mm.getAverageRatingByGenre();
            if (avgByGenre.empty()) {
                cout << "등록된 영화가 없습니다.\n";
                break;
            }
            cout << "장르별 평균 평점:\n";
            for (const auto& [genre, avg] : avgByGenre) {
                cout << "  " << genre << ": " << avg << "\n";
            }
            break;
        }
        default: 
            cout << "잘못된 메뉴 선택입니다.\n";
            break;
        }
    } catch (...) {
        cout << "오류가 발생했습니다.\n";
    } }

    mm.saveToFile("./data/movie.csv");
    um.saveToFile("./data/user.csv");
    rm.saveToFile("./data/rating.csv");
    return 0;
}
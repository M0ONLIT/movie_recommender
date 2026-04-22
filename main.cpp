#include "movie.h"
#include "rating.h"
#include "user.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

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

int main() {
    vector<Movie> movies;
    vector<User> users;
    vector<Rating> ratings;

    int choice;
    while (true) {
        showMenu();
        cin >> choice;

        if (choice == 0) break;

        switch (choice) {
            case 1: { // 영화 추가
                int id, year;
                string title, genre;
                cout << "ID: "; cin >> id;
                cin.ignore(); // 버퍼 비우기
                cout << "제목: "; getline(cin, title);
                cout << "장르: "; getline(cin, genre);
                cout << "개봉연도: "; cin >> year;
                movies.push_back(Movie(id, title, genre, year));
                break;
            }
            case 2: { // 제목으로 검색
                string title;
                cout << "검색할 제목: ";
                cin.ignore();
                getline(cin, title);
                bool found = false;
                for (const auto& m : movies) {
                    if (m.getTitle().find(title) != string::npos) {
                        cout << m << endl;
                        found = true;
                    }
                }
                if (!found) cout << "검색 결과가 없습니다.\n";
                break;
            }
            case 3: // 전체 목록 출력
                for (const auto& m : movies) cout << m << endl;
                break;
            case 4: { // 평점순 정렬 출력
                vector<Movie> sortedMovies = movies;
                // Movie 클래스에 정의한 operator< 활용 (평점 기준)
                // 내림차순(높은 평점순) 정렬을 위해 greater 사용
                sort(sortedMovies.begin(), sortedMovies.end());
                reverse(sortedMovies.begin(), sortedMovies.end()); 
                for (const auto& m : sortedMovies) cout << m;
                break;
            }
            case 5: { // 사용자 추가
                int id;
                string name, email;
                cout << "ID: "; cin >> id;
                cin.ignore();
                cout << "이름: "; getline(cin, name);
                cout << "이메일: "; getline(cin, email);
                users.push_back(User(id, name, email));
                break;
            }
            case 6: // 사용자 목록 출력
                for (const auto& u : users) cout << u;
                break;
            case 7: { // 평점 입력
                int uId, mId;
                double score;
                cout << "사용자 ID: "; cin >> uId;
                cout << "영화 ID: "; cin >> mId;
                cout << "평점(0-5): "; cin >> score;

                // 1. 평점 리스트에 추가
                ratings.push_back(Rating(uId, mId, score));
                
                // 2. 실제 영화 객체의 평균 평점 업데이트
                for (auto& m : movies) {
                    if (m.getId() == mId) {
                        m.addRating(score);
                        break;
                    }
                }
                break;
            }
            case 8: { // 영화별 평점 보기
                int mId;
                cout << "영화 ID: "; cin >> mId;
                for (const auto& r : ratings) {
                    if (r.getMovieId() == mId) cout << r << endl;
                }
                break;
            }
            default:
                cout << "잘못된 선택입니다.\n";
        }
    }

    return 0;
}
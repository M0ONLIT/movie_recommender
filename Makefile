# Makefile - Movie, User, Rating 통합 빌드
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -g
TARGET   = movie_app

# 모든 오브젝트 파일 목록 (새로운 클래스 추가)
OBJS     = main.o movie.o user.o rating.o

# 최종 실행 파일 빌드
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 각 소스 파일의 컴파일 규칙 및 헤더 의존성 명시
main.o: main.cpp movie.h user.h rating.h
	$(CXX) $(CXXFLAGS) -c $<

movie.o: movie.cpp movie.h
	$(CXX) $(CXXFLAGS) -c $<

user.o: user.cpp user.h
	$(CXX) $(CXXFLAGS) -c $<

rating.o: rating.cpp rating.h
	$(CXX) $(CXXFLAGS) -c $<

# 유틸리티 타겟
.PHONY: clean run

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
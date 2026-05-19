
CXX      = g++
CXXFLAGS = -std=c++17 -Wall -g -fexec-charset=utf-8
TARGET   = movie_app

OBJS     = main.o movie.o user.o rating.o \
           MovieManager.o UserManager.o RatingManager.o Recommender.o

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp MovieManager.h UserManager.h RatingManager.h Recommender.h
	$(CXX) $(CXXFLAGS) -c $<

movie.o: movie.cpp movie.h
	$(CXX) $(CXXFLAGS) -c $<

user.o: user.cpp user.h
	$(CXX) $(CXXFLAGS) -c $<

rating.o: rating.cpp rating.h
	$(CXX) $(CXXFLAGS) -c $<

MovieManager.o: MovieManager.cpp MovieManager.h BaseManager.h movie.h
	$(CXX) $(CXXFLAGS) -c $<

UserManager.o: UserManager.cpp UserManager.h BaseManager.h user.h
	$(CXX) $(CXXFLAGS) -c $<

RatingManager.o: RatingManager.cpp RatingManager.h BaseManager.h rating.h MovieManager.h UserManager.h
	$(CXX) $(CXXFLAGS) -c $<

Recommender.o: Recommender.cpp Recommender.h MovieManager.h UserManager.h RatingManager.h movie.h rating.h
	$(CXX) $(CXXFLAGS) -c $<

# 유틸리티 타겟
.PHONY: clean run

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
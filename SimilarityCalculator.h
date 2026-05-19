// SimilarityCalculator.h
#ifndef SIMILARITY_CALCULATOR_H
#define SIMILARITY_CALCULATOR_H

#include <vector>
#include "rating.h"

class SimilarityCalculator {
public:
    // 두 사용자의 평점 목록을 받아 유사도 점수를 반환한다. 클수록 더 비슷한 취향.
    static int calculate(const std::vector<Rating>& user1, const std::vector<Rating>& user2);
};

#endif

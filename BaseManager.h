// BaseManager.h
#pragma once
#include <string>

class BaseManager {
public:
    // 상속을 위한 가상 소멸자 (강의 자료 우측 하단 내용)
    virtual ~BaseManager() {} 

    // 자식 클래스들이 반드시 구현해야 하는 순수 가상 함수 (규칙 강제)
    virtual void loadFromFile(const std::string& filename) = 0;
    virtual void saveToFile(const std::string& filename) const = 0;
};
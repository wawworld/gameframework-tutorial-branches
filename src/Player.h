#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"

class Player : public GameObject {
public:
    // 🆕 NEW: 기본 클래스 생성자 호출하는 생성자
    Player(const std::string& textureID, int x, int y, int width, int height);

    // 🆕 NEW: 가상 소멸자
    virtual ~Player() {}

    // 🆕 NEW: 오버라이드된 가상 함수들
    virtual void update() override;     // 위치 업데이트 (입력 반영)
    virtual void handleInput();         // 입력 처리 함수
};

#endif // PLAYER_H
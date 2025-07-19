#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <SDL2/SDL.h>

class GameObject {
public:
    // 🆕 NEW: 생성자 - 텍스처 ID와 위치, 크기 정보 받음
    GameObject(const std::string& textureID, int x, int y, int width, int height);

    // 🆕 NEW: 가상 소멸자 - 올바른 파생 클래스 소멸 보장
    virtual ~GameObject() {}

    // 🆕 NEW: 가상 함수들 - 파생 클래스에서 오버라이드 가능
    virtual void update();              // 객체의 위치 갱신
    virtual void render(SDL_Renderer* pRenderer);  // 객체 렌더링
    virtual void clean();               // 리소스 정리

    // 🆕 NEW: 공통 유틸리티 함수들
    void setVelocity(int x, int y);     // 속도 설정
    void setPosition(int x, int y);     // 위치 설정

protected:
    // 🆕 NEW: 파생 클래스에서 접근 가능한 멤버들
    std::string m_textureID;            // 텍스처 ID
    int m_x, m_y;                       // 객체 위치
    int m_velocityX, m_velocityY;       // 객체 속도
    int m_width, m_height;              // 객체 크기
    int m_currentFrame, m_currentRow;   // 애니메이션 프레임 정보
};

#endif // GAMEOBJECT_H
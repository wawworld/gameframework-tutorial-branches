#pragma once
#include <SDL2/SDL.h>
#include <array>
#include "Vector2D.h"  // 🆕 NEW: 마우스 좌표 관리

// 🆕 NEW: 마우스 버튼 식별 열거형
enum mouse_buttons
{
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2
};

class InputHandler
{
public:
    ~InputHandler();

    static InputHandler* Instance();

    void update();
    void clean();

    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyOneDown(SDL_Scancode key) const;

    // 🆕 NEW: 마우스 상태 조회 메서드
    bool getMouseButtonState(int buttonNumber) const;
    Vector2D* getMousePosition() const;

private:
    InputHandler();

    static InputHandler* s_pInstance;
    const Uint8* m_keystates;
    std::array<bool, SDL_NUM_SCANCODES> m_lastKeyState;

    // 🆕 NEW: 마우스 상태 관리 멤버 변수
    Vector2D* m_mousePosition;
    std::array<bool, 3> m_mouseButtonStates;
    std::array<bool, 3> m_prevMouseButtonStates;

    // 🆕 NEW: 세분화된 이벤트 핸들러
    void onKeyDown(const SDL_Event& event);
    void onKeyUp(const SDL_Event& event);
    void onMouseMove(const SDL_Event& event);
    void onMouseButtonDown(const SDL_Event& event);
    void onMouseButtonUp(const SDL_Event& event);
};

typedef InputHandler TheInputHandler;
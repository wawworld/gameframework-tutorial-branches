#include "Game.h"
#include "InputHandler.h"
#include <iostream>

InputHandler* InputHandler::s_pInstance = nullptr;

InputHandler::InputHandler() : m_keystates(nullptr)
{
    m_lastKeyState.fill(false);

    // 🆕 NEW: 마우스 상태 초기화
    m_mousePosition = new Vector2D(0, 0);
    m_mouseButtonStates.fill(false);
    m_prevMouseButtonStates.fill(false);
}

InputHandler::~InputHandler()
{
    // 🆕 NEW: 마우스 메모리 정리
    delete m_mousePosition;
    m_mousePosition = nullptr;

    // 🆕 NEW: 포인터 초기화
    s_pInstance = nullptr;
    m_keystates = nullptr;
}

InputHandler* InputHandler::Instance()
{
    if (s_pInstance == nullptr)
    {
        s_pInstance = new InputHandler();
    }
    return s_pInstance;
}

void InputHandler::clean()
{
    // Add any necessary cleanup code here
}

void InputHandler::update()
{
    // 현재 키 상태 업데이트 전에 이전 상태 저장
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i)
    {
        m_lastKeyState[i] = (m_keystates != nullptr) ? m_keystates[i] == 1 : false;
    }

    // 🆕 NEW: 마우스 버튼 이전 상태 저장
    m_prevMouseButtonStates = m_mouseButtonStates;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            Game::Instance()->quit();
            return;
        case SDL_KEYDOWN:
            onKeyDown(event);
            break;
        case SDL_KEYUP:
            onKeyUp(event);
            break;
            // 🆕 NEW: 마우스 이벤트 처리
        case SDL_MOUSEMOTION:
            onMouseMove(event);
            break;
        case SDL_MOUSEBUTTONDOWN:
            onMouseButtonDown(event);
            break;
        case SDL_MOUSEBUTTONUP:
            onMouseButtonUp(event);
            break;
        }
    }

    // 현재 키보드 상태 업데이트
    m_keystates = SDL_GetKeyboardState(nullptr);
}

bool InputHandler::isKeyDown(SDL_Scancode key) const
{
    if (m_keystates != nullptr)
    {
        return m_keystates[key] == 1;
    }
    return false;
}

bool InputHandler::isKeyOneDown(SDL_Scancode key) const
{
    return isKeyDown(key) && !m_lastKeyState[key];
}

// 🆕 NEW: 마우스 상태 조회 메서드 구현
bool InputHandler::getMouseButtonState(int buttonNumber) const
{
    return m_mouseButtonStates[buttonNumber];
}

Vector2D* InputHandler::getMousePosition() const
{
    return m_mousePosition;
}

// 🆕 NEW: 이벤트 핸들러 메서드 구현
void InputHandler::onKeyDown(const SDL_Event& event)
{
    switch (event.key.keysym.scancode)
    {
    case SDL_SCANCODE_ESCAPE:
    case SDL_SCANCODE_Q:
        Game::Instance()->quit();
        break;
        // 추가적인 키 다운 이벤트 처리
    }
}

void InputHandler::onKeyUp(const SDL_Event& event)
{
    switch (event.key.keysym.scancode)
    {
    case SDL_SCANCODE_SPACE:
        // 스페이스바를 뗐을 때의 처리
        break;
        // 추가적인 키 업 이벤트 처리
    }
}

void InputHandler::onMouseMove(const SDL_Event& event)
{
    m_mousePosition->setX(event.motion.x);
    m_mousePosition->setY(event.motion.y);
}

void InputHandler::onMouseButtonDown(const SDL_Event& event)
{
    switch (event.button.button)
    {
    case SDL_BUTTON_LEFT:
        m_mouseButtonStates[LEFT] = true;
        break;
    case SDL_BUTTON_MIDDLE:
        m_mouseButtonStates[MIDDLE] = true;
        break;
    case SDL_BUTTON_RIGHT:
        m_mouseButtonStates[RIGHT] = true;
        break;
    }
}

void InputHandler::onMouseButtonUp(const SDL_Event& event)
{
    switch (event.button.button)
    {
    case SDL_BUTTON_LEFT:
        m_mouseButtonStates[LEFT] = false;
        break;
    case SDL_BUTTON_MIDDLE:
        m_mouseButtonStates[MIDDLE] = false;
        break;
    case SDL_BUTTON_RIGHT:
        m_mouseButtonStates[RIGHT] = false;
        break;
    }
}
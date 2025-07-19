#include "Game.h"
#include "InputHandler.h"

InputHandler* InputHandler::s_pInstance = nullptr;

InputHandler::InputHandler() : m_keystates(nullptr)
{
    m_lastKeyState.fill(false);
}

InputHandler::~InputHandler()
{
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

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            Game::Instance()->quit();
            return;
        case SDL_KEYDOWN:
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
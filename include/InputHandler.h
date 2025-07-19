#pragma once
#include <SDL2/SDL.h>
#include <array>

class InputHandler
{
public:
    ~InputHandler();

    static InputHandler* Instance();

    void update();
    void clean();

    bool isKeyDown(SDL_Scancode key) const;
    bool isKeyOneDown(SDL_Scancode key) const;

private:
    InputHandler();

    static InputHandler* s_pInstance;
    const Uint8* m_keystates;
    std::array<bool, SDL_NUM_SCANCODES> m_lastKeyState;
};

typedef InputHandler TheInputHandler;
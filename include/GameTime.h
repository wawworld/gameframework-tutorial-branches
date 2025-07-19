#ifndef TIME_H
#define TIME_H

#include <chrono>

class GameTime {
public:
    static GameTime* Instance() {
        if (s_pInstance == nullptr) {
            s_pInstance = new GameTime();
        }
        return s_pInstance;
    }

    void update();
    static float getDeltaTime() { return Instance()->m_deltaTime; }
    static float getTotalTime() { return Instance()->m_totalTime; }
    static int getFPS() { return Instance()->m_fps; }

private:
    GameTime();
    ~GameTime() = default;

    static GameTime* s_pInstance;

    std::chrono::high_resolution_clock::time_point m_lastFrameTime;
    float m_deltaTime;
    float m_totalTime;
    int m_fps;
    float m_fpsTimer;
    int m_frameCount;
};

typedef GameTime TheTime;

#endif // TIME_H
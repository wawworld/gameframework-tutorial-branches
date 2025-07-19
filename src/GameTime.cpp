#include "GameTime.h"

GameTime* GameTime::s_pInstance = nullptr;

GameTime::GameTime()
    : m_deltaTime(0.0f)
    , m_totalTime(0.0f)
    , m_fps(0)
    , m_fpsTimer(0.0f)
    , m_frameCount(0) {
    m_lastFrameTime = std::chrono::high_resolution_clock::now();
}

void GameTime::update() {
    auto currentTime = std::chrono::high_resolution_clock::now();

    m_deltaTime = std::chrono::duration<float>(currentTime - m_lastFrameTime).count();
    m_lastFrameTime = currentTime;

    m_totalTime += m_deltaTime;

    m_frameCount++;
    m_fpsTimer += m_deltaTime;

    if (m_fpsTimer >= 1.0f) {
        m_fps = m_frameCount;
        m_frameCount = 0;
        m_fpsTimer = 0.0f;
    }
}
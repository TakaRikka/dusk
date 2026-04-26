#pragma once

#include <queue>
#include <string>

namespace dusk {

class ImGuiAchievements {
public:
    void draw(bool& open);
    void notify(std::string name);

private:
    void showNotification();

    std::string m_notifyName;
    float m_notifyTimer = 0.f;
    std::queue<std::string> m_notifyQueue;
    static constexpr float NOTIFY_DURATION = 4.0f;
    static constexpr float NOTIFY_FADE_TIME = 0.5f;
};

} // namespace dusk

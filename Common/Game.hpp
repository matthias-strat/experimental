#pragma once

#include <functional>

class Game
{
public:
    std::function<void(const sf::Event&)> onEvent{nullptr};
    std::function<void(float)> onUpdate{nullptr};
    std::function<void()> onLoadContent{nullptr};
    std::function<void(sf::RenderTarget&)> onDraw{nullptr};

    inline Game(unsigned int windowWidth, unsigned int windowHeight, const std::string& windowTitle)
        : m_Window{{windowWidth, windowHeight}, windowTitle, sf::Style::Close}
    {
        m_Window.setVerticalSyncEnabled(true);
    }

    inline void run()
    {
        safeInvoke(onLoadContent);

        static const auto timeStep(sf::seconds(1.f/60.f));

        auto timeSinceLastUpdate(sf::Time::Zero);
        sf::Clock clock;

        while (m_Window.isOpen())
        {
            auto dt(clock.restart());
            timeSinceLastUpdate += dt;

            sf::Event event;
            while (m_Window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    m_Window.close();
                    break;
                }
                safeInvoke(onEvent, event);
            }

            while (timeSinceLastUpdate >= timeStep)
            {
                timeSinceLastUpdate -= timeStep;
                safeInvoke(onUpdate, timeStep.asSeconds());
            }

            m_Window.clear(sf::Color::White);
            safeInvoke(onDraw, m_Window);
            m_Window.display();
        }
    }

private:
    template <typename TFunc, typename... TArgs>
    inline static void safeInvoke(TFunc& func, TArgs&&... args)
    {
        if (func != nullptr) func(std::forward<TArgs>(args)...);
    }

    sf::RenderWindow m_Window;
};
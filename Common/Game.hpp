#pragma once

class Game
{
public:
    Func<void(const sf::Event&)> onEvent{nullptr};
    Func<void(float)> onUpdate{nullptr}, onUpdateVariable{nullptr};
    Func<void()> onLoadContent{nullptr};
    Func<void(int)> onFpsUpdated{nullptr};
    Func<void(sf::RenderTarget&)> onDraw{nullptr};

    inline Game(const std::string& windowTitle, unsigned int windowWidth = 1024, unsigned int windowHeight = 768) noexcept
        : m_Window{{windowWidth, windowHeight}, windowTitle},
          m_WindowWidth{windowWidth},
          m_WindowHeight{windowHeight}
    {
        m_Window.setVerticalSyncEnabled(true);
    }

    inline void run()
    {
        safeInvoke(onLoadContent);
        safeInvoke(onFpsUpdated, 0);

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

            updateFpsCounter(dt);
            safeInvoke(onUpdateVariable, dt.asSeconds());

            m_Window.clear(sf::Color::White);
            safeInvoke(onDraw, m_Window);
            m_Window.display();
        }
    }

    inline auto getWindowWidth() const noexcept { return m_WindowWidth; }
    inline auto getWindowHeight() const noexcept { return m_WindowHeight; }
    inline sf::RenderWindow& getWindow() noexcept { return m_Window; }

private:
    inline void updateFpsCounter(sf::Time deltaTime) noexcept
    {
        static const auto oneSecond(sf::seconds(1.f));
        m_FpsCounterTime += deltaTime;
        m_FpsCounter++;
        if (m_FpsCounterTime >= oneSecond)
        {
            m_LastFps = m_FpsCounter;
            m_FpsCounterTime -= oneSecond;
            m_FpsCounter = 0;
            safeInvoke(onFpsUpdated, m_LastFps);
        }
    }

    // Safely invoke an std::function (check whether it is null)
    template <typename TFunc, typename... TArgs>
    inline static void safeInvoke(TFunc& func, TArgs&&... args)
    {
        if (func != nullptr) func(FWD(args)...);
    }

    sf::RenderWindow m_Window;
    unsigned int m_WindowWidth, m_WindowHeight;
    sf::Time m_FpsCounterTime{sf::Time::Zero};
    std::size_t m_FpsCounter{0}, m_LastFps{0};
};
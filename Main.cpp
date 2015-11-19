#include <SFML/Graphics.hpp>

#include <memory>

#include <cassert>

// Aliases for simplicity
using Vec2u = sf::Vector2u;
using Vec2i = sf::Vector2i;
using Vec2f = sf::Vector2f;

namespace
{
    float dot(const Vec2f& v1, const Vec2f& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    float getLengthSquared(const Vec2f& v)
    {
        return dot(v, v);
    }

    float getLength(const Vec2f& v)
    {
        return std::sqrt(getLengthSquared(v));
    }

    void normalize(Vec2f& v)
    {
        auto length(getLength(v));
        if (length != 0) v /= length;
    }
}

struct AABB
{
    Vec2i position, halfSize;

public:
    AABB(const Vec2i& position, const Vec2i& halfSize)
        : position{position},
          halfSize{halfSize}
    {
    }

    AABB(int left, int right, int top, int bottom)
        : position{ left + (right - left) / 2, top + (bottom - top) / 2 },
          halfSize{(right - left) / 2, (bottom - top) / 2}
    {
    }

    void move(const Vec2i& offset)
    {
        position += offset;
    }
};

class Application
{
public:
    Application() 
        : m_Window{{1024, 768}, "SFML - AABB collision", sf::Style::Close},
          m_Position{100.f, 100.f},
          m_PrevPosition{m_Position}
    {
        m_Window.setVerticalSyncEnabled(true);
        m_Window.setKeyRepeatEnabled(false);

        auto result(m_DefaultFont.loadFromFile("C:/Windows/Fonts/verdana.ttf"));
        assert(result);

        m_PlayerShape.setSize({ 50.f, 50.f });
        m_PlayerShape.setOrigin(25.f, 25.f);
        m_PlayerShape.setFillColor(sf::Color::Black);
        m_PlayerShape.setPosition(m_Position);
    
        m_FpsCounterText.setFont(m_DefaultFont);
        m_FpsCounterText.setCharacterSize(16u);
        m_FpsCounterText.setPosition(3.f, 3.f);
        m_FpsCounterText.setColor(sf::Color::Black);
        m_FpsCounterText.setString("FPS: 0");

        m_PlayerPosText.setFont(m_DefaultFont);
        m_PlayerPosText.setCharacterSize(16u);
        m_PlayerPosText.setPosition(3.f, 20.f);
        m_PlayerPosText.setColor(sf::Color::Black);
        m_PlayerPosText.setString("Player X: " + std::to_string(m_Position.x) + "\nPlayer Y: " + std::to_string(m_Position.y));

    }
    
    void run()
    {
        static const auto timeStep(sf::seconds(1.f / 60.f));
        static const auto quarterSecond(sf::seconds(0.25f));

        auto accumulator(sf::seconds(0.f));
        sf::Clock clock;

        while (m_Window.isOpen())
        {
            auto dt(clock.restart());
            if (dt > quarterSecond) dt = quarterSecond;
            accumulator += dt;

            while (accumulator >= timeStep)
            {
                processInput();
                integrate(timeStep.asSeconds());
                accumulator -= timeStep;
            }

            updateFpsCounter(dt);

            auto alpha(accumulator / timeStep);
            interpolate(alpha);

            render();
        }
    }

private:
    void processInput()
    {
        sf::Event event;
        while (m_Window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) m_Window.close();
            handleEvent(event);
        }
    }

    void handleEvent(const sf::Event& event)
    {

    }

    void integrate(float timeStep)
    {
        Vec2f dir;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dir.x = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dir.x = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dir.y = -1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dir.y = 1;
        normalize(dir);

        static const auto velocity(300.f);

        m_PrevPosition = m_Position;
        m_Position.x += velocity * dir.x * timeStep;
        m_Position.y += velocity * dir.y * timeStep;
    }

    void updateFpsCounter(sf::Time dt)
    {
        static const auto oneSecond(sf::seconds(1.f));

        m_FpsCounterTime += dt;
        m_FpsCounterNum++;
        if (m_FpsCounterTime >= oneSecond)
        {
            m_FpsCounterTime -= oneSecond;
            m_FpsCounterText.setString("FPS: " + std::to_string(m_FpsCounterNum));
            m_FpsCounterNum = 0;
        }
    }

    void interpolate(float alpha)
    {
        if (abs(m_Position.x - m_PrevPosition.x) < 0.1f && abs(m_Position.y - m_PrevPosition.y) < 0.1f) return;

        Vec2f pos{m_Position.x*alpha + m_PrevPosition.x*(1.f - alpha),
                         m_Position.y*alpha + m_PrevPosition.y*(1.f - alpha)};
        m_PlayerPosText.setString("Player X: " + std::to_string(pos.x) + "\nPlayer Y: " + std::to_string(pos.y));
        m_PlayerShape.setPosition(pos);
    }

    void render()
    {
        m_Window.clear(sf::Color::White);
        m_Window.draw(m_WallShape);
        m_Window.draw(m_PlayerShape);
        
        m_Window.draw(m_FpsCounterText);
        m_Window.draw(m_PlayerPosText);
        m_Window.display();
    }

private:
    sf::RenderWindow m_Window;
    sf::Font m_DefaultFont;
    
    Vec2f m_Position;
    Vec2f m_PrevPosition;

    sf::RectangleShape m_PlayerShape;
    sf::RectangleShape m_WallShape;

    sf::Text m_FpsCounterText;
    sf::Time m_FpsCounterTime{sf::Time::Zero};
    std::size_t m_FpsCounterNum{0};

    sf::Text m_PlayerPosText;
};

int main()
{
    auto app(std::make_unique<Application>());
    app->run();
    return 0;
}


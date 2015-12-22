#include "../Common/Common.hpp"
#include <random>
#include <chrono>

constexpr int ballRadius{8};

struct Ball
{
    sf::CircleShape shape;
    Vec2f vel;
};

class PhysicsGame
{
public:
    inline explicit PhysicsGame(int shapeCount)
    {
        m_Game.onLoadContent = [this, &shapeCount]()
        {
            loadContent(shapeCount);
        };

        m_Game.onUpdate = [this](float ft)
        {
            update(ft);
        };

        m_Game.onDraw = [this](sf::RenderTarget& target)
        {
            draw(target);
        };
    }

    inline void run() { m_Game.run(); }

private:
    inline void loadContent(int shapeCount)
    {
        const auto windowWidth(m_Game.getWindowWidth());
        const auto windowHeight(m_Game.getWindowHeight());

        auto seed(std::chrono::system_clock::now().time_since_epoch().count());
        std::mt19937 el{seed};
        std::uniform_int_distribution<int> distWidth(ballRadius, windowWidth - ballRadius), distHeight(ballRadius, windowHeight - ballRadius),
            velDist(-450, 450);

        m_Balls.resize(shapeCount);
        for (auto i(0); i < shapeCount; ++i)
        {
            auto& b(m_Balls[i]);
            b.shape.setFillColor(sf::Color::Black);
            b.shape.setRadius(ballRadius);
            b.shape.setPosition(distWidth(el), distHeight(el));
            b.shape.setOrigin(ballRadius, ballRadius);
            b.vel.x = velDist(el);
            b.vel.y = velDist(el);
        }

    }

    inline void update(float ft)
    {        
        const auto windowWidth(m_Game.getWindowWidth());
        const auto windowHeight(m_Game.getWindowHeight());

        for (auto& b : m_Balls)
        {
            const auto& p(b.shape.getPosition());
            if (p.x < ballRadius) b.vel.x = -b.vel.x;
            else if (p.x > windowWidth - ballRadius) b.vel.x = -b.vel.x;

            if (p.y < ballRadius) b.vel.y = -b.vel.y;
            else if (p.y > windowHeight - ballRadius) b.vel.y = -b.vel.y;

            b.shape.move(b.vel*ft);
        }
    }

    inline void draw(sf::RenderTarget& target)
    {
        for (const auto& b : m_Balls) target.draw(b.shape);
    }

private:
    Game m_Game{"Physics"};
    std::vector<Ball> m_Balls;
};

int main()
{
    PhysicsGame{15}.run();
    return 0;
}
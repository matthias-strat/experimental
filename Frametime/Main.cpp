#include <SFML/Graphics.hpp>
#include <chrono>

constexpr unsigned int windowWidth{1024}, windowHeight{768};
constexpr float shapeWidth{50.f}, shapeHeight{50.f};
constexpr float shapeVelocity{5.f};

using HRClock = std::chrono::high_resolution_clock;

class Game
{
public:
    inline Game()
    {
        shape.setPosition(windowWidth/2.f, windowHeight/2.f);
        shape.setSize({shapeWidth, shapeHeight});
        shape.setOrigin(shapeWidth/2.f, shapeHeight/2.f);
        shape.setFillColor(sf::Color::Black);
    }

    inline void run()
    {
        auto tp1(HRClock::now());
        while (window.isOpen())
        {
            processInput();
            update();
            draw();
        }
    }

private:
    inline void processInput()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::A) xAxis = -1;
                else if (event.key.code == sf::Keyboard::D) xAxis = 1;
            }
            if (event.type == sf::Event::KeyReleased &&
                (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D))
            {
                xAxis = 0;
            }
        }
    }

    inline void update()
    {
        shape.move(shapeVelocity*xAxis, 0.f);
    }

    inline void draw()
    {
        window.clear(sf::Color::White);
        window.draw(shape);
        window.display();
    }

private:
    sf::RenderWindow window{{windowWidth, windowHeight}, "Frametime", sf::Style::Close};
    sf::RectangleShape shape;
    int xAxis{0};
};

int main()
{
    Game{}.run();
    return 0;
}
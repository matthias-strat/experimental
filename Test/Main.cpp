#include "../Common/Common.hpp"

constexpr unsigned int windowWidth{1024}, windowHeight{768};
constexpr int rectWidth{100}, rectHeight{100};

int main()
{    
    sf::RectangleShape shape;

    Game game{windowWidth, windowHeight, "SFML - Test"};
    game.onLoadContent = [&shape]()
    {
        shape.setSize({static_cast<float>(rectWidth), static_cast<float>(rectHeight)});
        shape.setPosition(windowWidth/2, windowHeight/2.f);
        shape.setOrigin(rectWidth/2.f, rectHeight/2.f);
        shape.setFillColor(sf::Color::Black);
    };
    game.onDraw = [&shape](sf::RenderTarget& target)
    {
        target.draw(shape);
    };
    
    game.run();
    return 0;
}
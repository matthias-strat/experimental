#include "Main.hpp"

constexpr int windowWidth{1024}, windowHeight{768};
constexpr int rectWidth{100}, rectHeight{100};

int main()
{
	test::testHeader();
	
	sf::RenderWindow window{{windowWidth, windowHeight}, "SFML", sf::Style::Close};
	window.setVerticalSyncEnabled(true);
	
	sf::RectangleShape shape;
	shape.setSize({static_cast<float>(rectWidth), static_cast<float>(rectHeight)});
	shape.setPosition(windowWidth/2, windowHeight/2.f);
	shape.setOrigin(rectWidth/2.f, rectHeight/2.f);
	shape.setFillColor(sf::Color::Black);
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}
		}
		
		window.clear(sf::Color::White);
		window.draw(shape);
		window.display();
	}
	return 0;
}

void test::testHeader()
{
	std::cout << "Yep works" << std::endl;
}
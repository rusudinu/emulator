#include <SFML/Graphics.hpp>

#define EMULATOR_WIDTH 192
#define EMULATOR_HEIGHT 40

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 500), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::RectangleShape rectShape;
    rectShape.setFillColor(sf::Color::Yellow);
    rectShape.setPosition(200,200);
    rectShape.setSize(sf::Vector2f(EMULATOR_WIDTH,EMULATOR_HEIGHT) *2.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(rectShape);
        window.display();
    }

    return 0;
}

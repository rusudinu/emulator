#include <SFML/Graphics.hpp>

#define EMULATOR_WIDTH 192
#define EMULATOR_HEIGHT 40

int main()
{
    sf::RenderWindow window(sf::VideoMode(780, 332), "Atari 2600");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::RectangleShape rectShape;
    rectShape.setFillColor(sf::Color::Yellow);
    rectShape.setPosition(200,125);
    rectShape.setSize(sf::Vector2f(EMULATOR_WIDTH,EMULATOR_HEIGHT) *2.f);


    sf::Texture bodyTexture;
    if(!bodyTexture.loadFromFile("assets/BACKGROUND.PNG")){
    // Handle an error.
    }
    sf::Sprite bodySprite(bodyTexture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        window.clear();
        window.draw(bodySprite);
        window.draw(rectShape);
        window.display();
    }

    return 0;
}

#include <SFML/Graphics.hpp>

#define EMULATOR_WIDTH 192
#define EMULATOR_HEIGHT 40

// Instructions per second
#define EMULATOR_IPS 6507

#include "ButtonBoard.h"
#include "Interpreter.hpp"

#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(780, 332), "Atari 2600");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::RectangleShape rectShape;
    rectShape.setFillColor(sf::Color::Yellow);
    rectShape.setPosition(200,125);
    rectShape.setSize(sf::Vector2f(EMULATOR_WIDTH, EMULATOR_HEIGHT) *2.f);

    ButtonBoard bBoard;

    sf::Texture bodyTexture;
    if(!bodyTexture.loadFromFile("assets/BACKGROUND.PNG")){ /* error */ }
    sf::Sprite bodySprite(bodyTexture);

    Interpreter interpreter;
    interpreter.loadROMFromFile("game.rom");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            bBoard.onEvent(event);
        }

        // std::cout << (int)bBoard.getInputByte() << std::endl;

        window.clear(sf::Color::White);
        window.draw(bodySprite); //THE BODY OF THE CONSOLE
        window.draw(rectShape);
        window.draw(bBoard);
        window.display();
    }

    return 0;
}

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


    /**************************  THE BODY OF THE CONSOLE **************************/
    sf::Texture bodyTexture;
    if(!bodyTexture.loadFromFile("assets/BACKGROUND.PNG")){
    // Handle an error.
    }
    sf::Sprite bodySprite(bodyTexture);
    /**************************  THE BODY OF THE CONSOLE **************************/


    /**************************  THE LEFT BUTTONS OF THE CONSOLE **************************/

    /**************************  THE LEFT BUTTONS OF THE CONSOLE **************************/

    /**************************  THE RIGHT BUTTONS OF THE CONSOLE **************************/
     sf::Texture greenButtonTexture;
     sf::Texture blueButtonTexture;
     sf::Texture pinkButtonTexture;
     sf::Texture redButtonTexture;

    if(!greenButtonTexture.loadFromFile("assets/buttonsRight/BACKGROUND.PNG")){
    // Handle an error.
    }

    if(!blueButtonTexture.loadFromFile("assets/BACKGROUND.PNG")){
    // Handle an error.
    }
    if(!pinkButtonTexture.loadFromFile("assets/BACKGROUND.PNG")){
    // Handle an error.
    }

    if(!redButtonTexture.loadFromFile("assets/BACKGROUND.PNG")){
    // Handle an error.
    }

    sf::Sprite greenButtonSprite(greenButtonTexture);
    sf::Sprite blueButtonSprite(blueButtonTexture);
    sf::Sprite pinkButtonSprite(pinkButtonTexture);
    sf::Sprite redButtonSprite(redButtonTexture);
    /**************************  THE RIGHT BUTTONS OF THE CONSOLE **************************/


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        window.clear(sf::Color::White);
        window.draw(bodySprite); //THE BODY OF THE CONSOLE
        window.draw(rectShape);
        window.display();
    }

    return 0;
}

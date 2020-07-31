#include <SFML/Graphics.hpp>

#include "Defines.hpp"

#include "ButtonBoard.h"
#include "Interpreter.hpp"

#include <iostream>

#include <sstream>

std::string getStringFromByte( unsigned char b ){

    // 0b00011010
    std::string str = "";
    //std::cout << ((int)(b & 0b10000001) != 0) << std::endl;
    //std::cout << ((int)(b & 0b01000010) != 0)<< std::endl;

    str += '0' + ((int)(b & 0b10000000) != 0);
    str += '0' + ((int)(b & 0b01000000) != 0);
    str += '0' + ((int)(b & 0b00100000) != 0);
    str += '0' + ((int)(b & 0b00010000) != 0);
    str += ' ';
    str += '0' + ((int)(b & 0b00001000) != 0);
    str += '0' + ((int)(b & 0b00000100) != 0);
    str += '0' + ((int)(b & 0b00000010) != 0);
    str += '0' + ((int)(b & 0b00000001) != 0);
    return str;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(780, 532), "ACeTare 2600 PLUS PRO MAX");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    window.setFramerateLimit(60);

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
    //interpreter.interpret(EMULATOR_IPS);
    sf::RenderTexture& rtex = interpreter.getRenderTexture();

    sf::Sprite renderSprite;
    renderSprite.setScale(2.f,2.f);
    renderSprite.setPosition(200,125);

    sf::Font debugFont;
    debugFont.loadFromFile("assets/fonts/VT323-Regular.ttf");

    sf::Text inputByteText;
    inputByteText.setFont(debugFont);
    unsigned char bbb = 0b00011010;
    inputByteText.setString( getStringFromByte(bbb) );
    inputByteText.setFillColor(sf::Color::Black);
    inputByteText.setPosition(70,360);


    sf::Text inputByteLabel;
    inputByteLabel.setFont(debugFont);
    inputByteLabel.setString( "IN = " );
    inputByteLabel.setFillColor(sf::Color::Black);
    inputByteLabel.setPosition(0,360);

    sf::Text stackPointerText;
    stackPointerText.setFont(debugFont);
    stackPointerText.setString( getStringFromByte(bbb) );
    stackPointerText.setFillColor(sf::Color::Black);
    stackPointerText.setPosition(70,390);


    sf::Text stackPointerLabel;
    stackPointerLabel.setFont(debugFont);
    stackPointerLabel.setString( "SP = " );
    stackPointerLabel.setFillColor(sf::Color::Black);
    stackPointerLabel.setPosition(0,390);


    sf::Text fpsDisplay;
    fpsDisplay.setFont(debugFont);
    fpsDisplay.setString( "FPS" );
    fpsDisplay.setFillColor(sf::Color::Black);
    fpsDisplay.setPosition(0,330);


    sf::Clock renderClock;
    sf::Time time;


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

        interpreter.setInputRegister(bBoard.getInputByte());
        inputByteText.setString( getStringFromByte(bBoard.getInputByte()) );

        interpreter.interpret(EMULATOR_IPS);

        stackPointerText.setString( getStringFromByte(interpreter.getSP()) );

        /*
        float currentTime = clock.restart().asSeconds();
        float fps = 1.f / (currentTime - lastTime);
        lastTime = currentTime;
        */

        //std::cout << fps << std::endl;

        time = renderClock.getElapsedTime();
        float fFps = 1000000 / time.asMicroseconds();
        std::stringstream s;
        s << fFps << " fps";
        fpsDisplay.setString(s.str());
        renderClock.restart();

        rtex.display();
        renderSprite.setTexture(rtex.getTexture());
        window.draw(renderSprite);
        window.draw(inputByteText);
        window.draw(inputByteLabel);
        window.draw(stackPointerText);
        window.draw(stackPointerLabel);
        window.draw(fpsDisplay);
        window.display();

    }

    return 0;
}

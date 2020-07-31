#include "GUIButton.hpp"

#include <iostream>

GUIButton::GUIButton( BUTTONS button, sf::Vector2f position )
{
    btnType = button;

    switch(button){
        case BUTTONS::DPAD_UP:
            backgroundTex.loadFromFile("assets/buttons left/BLL.PNG");
            pressedTex.loadFromFile("assets/buttons left/BLLP.PNG");
            setRotation(90);
            break;
        case BUTTONS::DPAD_DOWN:
            backgroundTex.loadFromFile("assets/buttons left/BLL.PNG");
            pressedTex.loadFromFile("assets/buttons left/BLLP.PNG");
            setRotation(-90);
            break;

        case BUTTONS::DPAD_LEFT:
            backgroundTex.loadFromFile("assets/buttons left/BLL.PNG");
            pressedTex.loadFromFile("assets/buttons left/BLLP.PNG");
            break;
        case BUTTONS::DPAD_RIGHT:
            backgroundTex.loadFromFile("assets/buttons left/BLL.PNG");
            pressedTex.loadFromFile("assets/buttons left/BLLP.PNG");
            setRotation(180);
            break;
        case BUTTONS::COLOR_UP:
            backgroundTex.loadFromFile("assets/buttons right/BRTP.PNG");
            pressedTex.loadFromFile("assets/buttons right/BRT.PNG");
            break;
        case BUTTONS::COLOR_DOWN:
            backgroundTex.loadFromFile("assets/buttons right/BRBP.PNG");
            pressedTex.loadFromFile("assets/buttons right/BRB.PNG");
            break;
        case BUTTONS::COLOR_LEFT:
            backgroundTex.loadFromFile("assets/buttons right/BRLP.PNG");
            pressedTex.loadFromFile("assets/buttons right/BRL.PNG");
            break;
        case BUTTONS::COLOR_RIGHT:
            backgroundTex.loadFromFile("assets/buttons right/BRRP.PNG");
            pressedTex.loadFromFile("assets/buttons right/BRR.PNG");
            break;
    }
    setTexture(backgroundTex);
    setPosition(position);
    pressed = false;
}

GUIButton::~GUIButton()
{

}

bool GUIButton::doesPositionIntersectThis( int x, int y ) {
    sf::FloatRect bounds = getGlobalBounds();

    return bounds.left < x && (bounds.left + bounds.width) > x &&
            bounds.top < y && (bounds.top + bounds.height) > y;
}

void GUIButton::setPressed(bool isPressed){
    if (isPressed){
        setTexture(pressedTex);
    } else {
        setTexture(backgroundTex);
    }
    pressed = isPressed;
}

sf::Keyboard::Key getKeyCode( BUTTONS button ){
    switch(button){
        case BUTTONS::DPAD_UP:
            return sf::Keyboard::W;
            break;
        case BUTTONS::DPAD_DOWN:
            return sf::Keyboard::S;
            break;
        case BUTTONS::DPAD_LEFT:
            return sf::Keyboard::A;
            break;
        case BUTTONS::DPAD_RIGHT:
            return sf::Keyboard::D;
            break;
        case BUTTONS::COLOR_UP:
            return sf::Keyboard::I;
            break;
        case BUTTONS::COLOR_DOWN:
            return sf::Keyboard::K;
            break;
        case BUTTONS::COLOR_LEFT:
            return sf::Keyboard::J;
            break;
        case BUTTONS::COLOR_RIGHT:
            return sf::Keyboard::L;
            break;
    }
}

void GUIButton::onEvent(sf::Event event){
    if ( event.type == sf::Event::MouseButtonPressed ) {
        if ( doesPositionIntersectThis(event.mouseButton.x, event.mouseButton.y )  ) {
            setPressed(true);
        }
        return;
    }
    if ( event.type == sf::Event::MouseButtonReleased ) {
        setPressed(false);
        return;
    }
    int keyCode = getKeyCode(btnType);
    if ( event.type == sf::Event::KeyPressed ){
        if ( event.key.code == keyCode ){
            setPressed(true);
        }
        return;
    }
    if ( event.type == sf::Event::KeyReleased ){
        if ( event.key.code == keyCode ){
            setPressed(false);
        }
        return;
    }
}

char GUIButton::getBinaryEncoding(){
    if ( pressed ) {
        return 1 << ((int)btnType);
    }
    return 0;
}

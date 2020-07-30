#include "ButtonBoard.h"

int buttonXes[] = {130, 90, 40, 180, 600, 600, 600, 600};
int buttonYes[] = {90, 230, 140, 180, 50, 80, 110, 140};

ButtonBoard::ButtonBoard()
{
    for ( int i = 0; i < 8; i++ ){
        buttons[i] = new GUIButton((BUTTONS)i, sf::Vector2f(buttonXes[i], buttonYes[i]));
    }
}

ButtonBoard::~ButtonBoard()
{
    for ( int i = 0; i < 8; i++ ){
        delete buttons[i];
    }
}

void ButtonBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for ( int i = 0; i < 8; i++ ){
        target.draw((*buttons[i]));
    }
}

void ButtonBoard::onEvent(sf::Event event)
{
    for ( int i = 0; i < 8; i++ ){
        buttons[i]->onEvent(event);
    }
}

char ButtonBoard::getInputByte()
{
    char inputByte = 0;
    for ( int i = 0; i < 8; i++ ){
        inputByte |= buttons[i]->getBinaryEncoding();
    }
    return inputByte;
}

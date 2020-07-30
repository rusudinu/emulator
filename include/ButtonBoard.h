#ifndef BUTTONBOARD_HPP
#define BUTTONBOARD_HPP

#include "GUIButton.hpp"

class ButtonBoard : public sf::Drawable
{
    private:
        GUIButton * buttons[8];
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    public:
        ButtonBoard();
        virtual ~ButtonBoard();
        void onEvent(sf::Event event);
        char getInputByte();
};

#endif // BUTTONBOARD_HPP

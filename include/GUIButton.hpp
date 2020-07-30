#ifndef GUIBUTTON_HPP
#define GUIBUTTON_HPP

#include <SFML/Graphics.hpp>

enum class BUTTONS {
    DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT,
    COLOR_UP, COLOR_DOWN, COLOR_LEFT, COLOR_RIGHT
};

class GUIButton : public sf::Sprite
{
    private:
        sf::Texture backgroundTex;
        sf::Texture pressedTex;
        BUTTONS btnType;
        bool pressed;
        void setPressed(bool isPressed);
    public:
        GUIButton( BUTTONS button, sf::Vector2f position);
        virtual ~GUIButton();
        void onEvent(sf::Event event);
        bool doesPositionIntersectThis( int x, int y );
        char getBinaryEncoding();
};

#endif // GUIBUTTON_HPP

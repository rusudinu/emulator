#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <string>
#include <SFML/Graphics.hpp>

class Interpreter
{
    private:
        unsigned char RA, RB, RC, RD, RE, RF; // General-purpose registries
        unsigned short PC; // Program Counter Registry
        unsigned short SP; // Stack Pointer Registry
        unsigned short IN; // Input Registry
        unsigned short RET; // Return Address Registry

        sf::RenderTexture renderTexture;
        char * ROM;
        unsigned char * RAM;

        void updateSP();

        unsigned char * getSpecialRegistry(char index);
        unsigned char * getRegistry( char index );

        short stackPointer;

        sf::Color getColor( char color );
        void clearCanvas(char color);
        void drawRectangle(unsigned char x, unsigned char y, unsigned char width, unsigned char height, unsigned char color);
        void drawLine(char x, char y, char tx, char ty, char color);

    public:
        Interpreter();
        virtual ~Interpreter();
        bool loadROMFromFile(std::string path);
        sf::RenderTexture& getRenderTexture();
        void interpret(int instructions);
        void setInputRegister( char inputByte );
        unsigned short getSP();
};

#endif // INTERPRETER_HPP

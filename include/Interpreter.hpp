#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <string>
#include <SFML/Graphics.hpp>

class Interpreter
{
    private:
        char RA, RB, RC, RD, RE, RF; // General-purpose registries
        short PC; // Program Counter Registry
        short SP; // Stack Pointer Registry
        short IN; // Input Registry
        short RET; // Return Address Registry

        sf::RenderTexture renderTexture;
        char * ROM;

        void clearCanvas(char color);
        void drawRectangle(char x, char y, char width, char height, char color);
        void drawLine(char x, char y, char tx, char ty, char color);

    public:
        Interpreter();
        virtual ~Interpreter();
        bool loadROMFromFile(std::string path);
        sf::RenderTexture& getRenderTexture();
        void interpret(int instructions);
        void setInputRegister( char inputByte );
};

#endif // INTERPRETER_HPP

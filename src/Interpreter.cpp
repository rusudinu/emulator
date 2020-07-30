#include "Interpreter.hpp"

#include <fstream>
#include <iostream>

#include "Defines.hpp"

Interpreter::Interpreter() {
    ROM = 0;
    renderTexture.create(EMULATOR_WIDTH, EMULATOR_HEIGHT);
    renderTexture.clear();
    // renderTexture.display();
}

Interpreter::~Interpreter() {
    if (!ROM) {
        delete ROM;
    }
}

bool Interpreter::loadROMFromFile(std::string path) {
    std::ifstream file(path.c_str(), std::ios::binary | std::ios::ate);
    if (!file){
        std::cout << "[ROM] failed to load : " << path << std::endl;
    } else {
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        ROM = new char[size];
        // std::vector<char> buffer(size);
        if (file.read(ROM, size))
        {
            std::cout << "[ROM] Loaded : " << size << " bytes." << std::endl;
        }
    }
}

sf::RenderTexture& Interpreter::getRenderTexture() {
    return renderTexture;
}

union Instruction{
    unsigned int int32; // 4 byte instruction
    short h[2]; // 2 halves of 2 bytes
    char b[4]; // 4 bytes
};

char * Interpreter::getRegistry(char index) {
    switch(index){
        case 0:
            return &RA;
        case 1:
            return &RB;
        case 2:
            return &RC;
        case 3:
            return &RD;
        case 4:
            return &RE;
        case 5:
            return &RF;
        default:
            std::cout << "[ERR] Attempting to retrieve a registry that doesn't exist! " << (int)index << std::endl;
            break;
    }
}

void Interpreter::interpret(int instructions) {
    Instruction currentINSTR;
    for ( int i = 0; i < instructions; i++ ){

        currentINSTR.int32 = *( (unsigned int * )( ROM + PC*4 ));
        // std::cout << currentINSTR.int32 << std::endl;

        switch( currentINSTR.b[0] ){
            case 0: // LBI
                std::cout << "LBI " << (int)currentINSTR.b[1] << " " << (int)currentINSTR.b[2] << std::endl;
                getRegistry(currentINSTR.b[1])[0] = currentINSTR.b[2];
                PC++;
                break;
            case 8:
                renderTexture.clear(sf::Color::Red);
                break;
        }
    }
}

void Interpreter::setInputRegister(char inputByte) {
    IN = inputByte;
}

void Interpreter::clearCanvas( char color ) {
    renderTexture.clear();
}

void Interpreter::drawRectangle(char x, char y, char width, char height, char color) {

}

void Interpreter::drawLine(char x, char y, char tx, char ty, char color) {

}

sf::Color c0 = sf::Color(100,100,100);// 0111
sf::Color c1 = sf::Color(100,100,100);// 1000
sf::Color c2 = sf::Color(100,100,100);// 1001
sf::Color c3 = sf::Color(100,100,100);// 1010
sf::Color c4 = sf::Color(100,100,100);// 1011
sf::Color c5 = sf::Color(100,100,100);// 1100
sf::Color c6 = sf::Color(100,100,100);// 1101
sf::Color c7 = sf::Color(100,100,100);// 1110

sf::Color pallette[] = {
    sf::Color::Black,             // 0000
    sf::Color::Red,               // 0001
    sf::Color::Green,             // 0010
    sf::Color::Blue,              // 0011
    sf::Color::Yellow,            // 0100
    sf::Color::Cyan,              // 0101
    sf::Color::Magenta,           // 0110
    c0,                           // 0111
    c1,                           // 1000
    c2,                           // 1001
    c3,                           // 1010
    c4,                           // 1011
    c5,                           // 1100
    c6,                           // 1101
    c7,                           // 1110
    sf::Color::White              // 1111
};

sf::Color Interpreter::getColor(char color){
    if ( color < 16 && color >= 0 ){
        return pallette[color];
    }
    return sf::Color::Black;
}


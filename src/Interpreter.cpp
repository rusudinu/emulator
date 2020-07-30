#include "Interpreter.hpp"

#include <fstream>
#include <iostream>

Interpreter::Interpreter() {
    ROM = 0;
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

void Interpreter::interpret(int instructions) {
    for ( int i = 0; i < instructions; i++ ){

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

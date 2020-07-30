#include "Interpreter.hpp"

#include <fstream>
#include <iostream>

#include "Defines.hpp"



Interpreter::Interpreter() {
    ROM = 0;
    renderTexture.create(EMULATOR_WIDTH, EMULATOR_HEIGHT);
    renderTexture.clear();
    // renderTexture.display();
    RAM = new unsigned char[EMULATOR_RAM];
}

Interpreter::~Interpreter() {
    if (!ROM) {
        delete ROM;
    }
    delete RAM;
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
    unsigned short h[2]; // 2 halves of 2 bytes
    unsigned char b[4]; // 4 bytes
};

enum class REG_TYPE{
    RA, RB, RC, RD, RE, RF, PC, SP, IN, RET
};

unsigned char * Interpreter::getRegistry(char index) {
    switch(index){
        case (char)REG_TYPE::RA:
            return &RA;
        case (char)REG_TYPE::RB:
            return &RB;
        case (char)REG_TYPE::RC:
            return &RC;
        case (char)REG_TYPE::RD:
            return &RD;
        case (char)REG_TYPE::RE:
            return &RE;
        case (char)REG_TYPE::RF:
            return &RF;
        /* // NEED MOVH / MOVL ( for Highs and Lows )
        case (char)REG_TYPE::PC:
            return &PC;
        case (char)REG_TYPE::SP:
            return &SP;
        */
        case (char)REG_TYPE::IN:
            return ((unsigned char *) &IN); // HOTFIX TO TEST SOMETHING. TODO : CHANGE PROPERLY!
        /* // NEED MOVH / MOVL ( for Highs and Lows )
        case (char)REG_TYPE::RET:
            return &RET;
        */
        default:
            std::cout << "[ERR] Attempting to retrieve a registry that doesn't exist! " << (int)index << std::endl;
            break;
    }
}

enum class INSTRUCTION_TYPE
{
    LBI, LB, SB, CALL, JUMP, SYSCALL, MOV, ADD, BEQ, BNE, BGE, BLE, BGT, BLT
};

union LABEL{
    unsigned short int SHORT;
    unsigned char BYTE[2];
};

void Interpreter::interpret(int instructions) {
    Instruction currentINSTR;
    for ( int i = 0; i < instructions; i++ ){

        currentINSTR.int32 = *( (unsigned int * )( ROM + PC*4 ));
        // std::cout << currentINSTR.int32 << std::endl;

        LABEL label;

        switch( currentINSTR.b[0] ){
            case (char)INSTRUCTION_TYPE::LBI: // Load Byte Immediate Value
                #ifdef VERBOSE
                std::cout << "LBI " << (int)currentINSTR.b[1] << " " << (int)currentINSTR.b[2] << std::endl;
                #endif // VERBOSE
                getRegistry(currentINSTR.b[1])[0] = currentINSTR.b[2];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::LB: // Load Byte
                getRegistry(currentINSTR.b[1])[0] = RAM[currentINSTR.b[2]];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::SB: // Store Byte
                RAM[getRegistry(currentINSTR.b[1])[0]] = getRegistry(currentINSTR.b[2])[0];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::CALL: // Call Subroutine
                RET = PC+1;
                label.BYTE[0] = currentINSTR.b[2];
                label.BYTE[1] = currentINSTR.b[3];
                #ifdef VERBOSE
                std::cout << "CALL to instruction " << label.SHORT << std::endl;
                #endif // VERBOSE
                PC = label.SHORT;
                break;
            case (char)INSTRUCTION_TYPE::JUMP: // Jump to label
                label.BYTE[0] = currentINSTR.b[2];
                label.BYTE[1] = currentINSTR.b[3];
                #ifdef VERBOSE
                std::cout << "JUMP to instruction " << label.SHORT << std::endl;
                #endif // VERBOSE
                PC = label.SHORT;
                break;
            case (char)INSTRUCTION_TYPE::SYSCALL:{ // System Call
                #ifdef VERBOSE
                std::cout << "SYSCALL " << (int)RA << std::endl;
                #endif // VERBOSE
                unsigned char SYSCODE = RA;

                switch(SYSCODE){
                    case 1: // DRAW RECTANGLE
                        drawRectangle(RB,RC,RD,RE,RF);
                        break;
                    case 255: // HALT
                        i = instructions;
                        break;
                    default:
                        std::cout << "WARNING! Unkown SYSCODE " << (int)SYSCODE << std::endl;
                }
                PC++;
                break;
            }
            case (char)INSTRUCTION_TYPE::MOV: // Move byte ( copy )
                #ifdef VERBOSE
                std::cout << "MOV " << (int)getRegistry(currentINSTR.b[1])[0] << " " << (int)getRegistry(currentINSTR.b[2])[0] << std::endl;
                #endif // VERBOSE
                getRegistry(currentINSTR.b[1])[0] = getRegistry(currentINSTR.b[2])[0];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::ADD: // Addition
                #ifdef VERBOSE
                std::cout << "ADD " << (int)getRegistry(currentINSTR.b[2])[0] << " + " << (int)getRegistry(currentINSTR.b[3])[0] << std::endl;
                #endif // VERBOSE
                getRegistry(currentINSTR.b[1])[0] = getRegistry(currentINSTR.b[2])[0] +
                                                    getRegistry(currentINSTR.b[3])[0];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::BEQ: // Branch if equal

                break;
            case (char)INSTRUCTION_TYPE::BNE: // Branch if not equal

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

sf::RectangleShape rectShapeStencil(sf::Vector2f(120, 50));

void Interpreter::drawRectangle(unsigned char x, unsigned char y, unsigned char width, unsigned char height, unsigned char color) {
    int X = x;
    int Y = y;
    int W = width;
    int H = height;
    sf::Color C = getColor(color);
    rectShapeStencil.setPosition(X,Y);
    rectShapeStencil.setSize(sf::Vector2f(W,H));
    rectShapeStencil.setFillColor(C);
    renderTexture.draw(rectShapeStencil);
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


#include "Interpreter.hpp"

#include <fstream>
#include <iostream>

#include "Defines.hpp"

// #define VERBOSE

void Interpreter::updateSP()
{
    SP = stackPointer & 0b111111111111111;
}

unsigned short Interpreter::getSP()
{
    return SP;
}


Interpreter::Interpreter() {
    ROM = 0;
    renderTexture.create(EMULATOR_WIDTH, EMULATOR_HEIGHT);
    renderTexture.clear();
    // renderTexture.display();
    RAM = new unsigned char[EMULATOR_RAM];
    stackPointer = EMULATOR_RAM;
    RA = RB = RC = RD = RE = RF = 0;
    PC = 0;
    updateSP();
    RET = 0;
}

Interpreter::~Interpreter() {
    if (!ROM) {
        delete ROM;
    }
    delete RAM;
}

union LABEL{
    unsigned short int SHORT;
    unsigned char BYTE[2];
};

LABEL PCStart;

bool Interpreter::loadROMFromFile(std::string path) {
    std::ifstream file(path.c_str(), std::ios::binary | std::ios::ate);
    if (!file){
        std::cout << "[ROM] failed to load : " << path << std::endl;
        return false;
    } else {
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        ROM = new char[size];
        // std::vector<char> buffer(size);
        if (file.read(ROM, size))
        {
            std::cout << "[ROM] Loaded : " << size << " bytes." << std::endl;

            LABEL ROMHeader;
            ROMHeader.SHORT = ((unsigned short int *)ROM)[0];

            PCStart.BYTE[1] = ROMHeader.BYTE[0];
            PCStart.BYTE[0] = ROMHeader.BYTE[1];
            std::cout << "STARTING AT LINE " <<  PCStart.SHORT << std::endl;
            PC = PCStart.SHORT;

            return true;
        }
    }
    return true;
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

unsigned char * Interpreter::getSpecialRegistry(char index) {
    switch(index){
        case (char)REG_TYPE::PC:
            return ((unsigned char *) &PC);
            break;
        case (char)REG_TYPE::SP:
            return ((unsigned char *) &SP);
            break;
        case (char)REG_TYPE::RET:
            return ((unsigned char *) &RET);
            break;
        default:
            return 0;
            break;
    }
}

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
            return ((unsigned char *) &IN); // HOTFIX : INTERPRETS ONLY THE LOWER BYTE
        /* // NEED MOVH / MOVL ( for Highs and Lows )
        case (char)REG_TYPE::RET:
            return &RET;
        */
        default:
            std::cout << "[ERR] Attempting to retrieve a registry that doesn't exist! " << (int)index << std::endl;
            return 0;
            break;
    }
}

enum class INSTRUCTION_TYPE
{
    LBI, LB, SB, CALL, JUMP, SYSCALL, MOV, ADD, BEQ, BNE, BGE, BLE, BGT, BLT, SBIX, LBIX, JRT, PUSH, POP, RRA, RRB, RRC, RRD, RRE, RRF, XOR
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
                getRegistry(currentINSTR.b[1])[0] = RAM[getRegistry(currentINSTR.b[2])[0]];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::SB: // Store Byte
                RAM[getRegistry(currentINSTR.b[1])[0]] = getRegistry(currentINSTR.b[2])[0];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::CALL: // Call Subroutine
                RET = PC+1;
                label.BYTE[1] = currentINSTR.b[2];
                label.BYTE[0] = currentINSTR.b[3];
                #ifdef VERBOSE
                std::cout << "CALL to instruction " << label.SHORT << std::endl;
                #endif // VERBOSE
                PC = label.SHORT + PCStart.SHORT;
                break;
            case (char)INSTRUCTION_TYPE::JUMP: // Jump to label
                label.BYTE[1] = currentINSTR.b[2];
                label.BYTE[0] = currentINSTR.b[3];

                PC = label.SHORT + PCStart.SHORT;
                #ifdef VERBOSE
                std::cout << "JUMP to instruction " << PCStart.SHORT << " " << label.SHORT << std::endl;
                #endif // VERBOSE
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
                if ( getRegistry(currentINSTR.b[1])[0] == getRegistry(currentINSTR.b[2])[0] ){
                    PC++;
                    //std::cout << "BRANCHED CLOSE" << std::endl;
                } else {
                    PC += 2;
                    //std::cout << "BRANCHED FAR" << std::endl;
                }
                break;
            case (char)INSTRUCTION_TYPE::BNE: // Branch if not equal
                if ( getRegistry(currentINSTR.b[1])[0] != getRegistry(currentINSTR.b[2])[0] ){
                    PC++;
                } else {
                    PC += 2;
                }
                break;
            case (char)INSTRUCTION_TYPE::BGE: // Branch if greater or equal
                if ( getRegistry(currentINSTR.b[1])[0] >= getRegistry(currentINSTR.b[2])[0] ){
                    PC++;
                } else {
                    PC += 2;
                }
                break;
            case (char)INSTRUCTION_TYPE::BLE: // Branch if less or equal
                if ( getRegistry(currentINSTR.b[1])[0] <= getRegistry(currentINSTR.b[2])[0] ){
                    PC++;
                } else {
                    PC += 2;
                }
                break;
            case (char)INSTRUCTION_TYPE::BGT: // Branch if greater than
                //std::cout << "BGT : " << (int)getRegistry(currentINSTR.b[1])[0] << " " << (int)getRegistry(currentINSTR.b[2])[0] << std::endl;
                if ( getRegistry(currentINSTR.b[1])[0] > getRegistry(currentINSTR.b[2])[0] ){
                    PC++;
                    //std::cout << "BRANCHED CLOSE" << std::endl;
                } else {
                    PC += 2;
                    //std::cout << "BRANCHED FAR" << std::endl;
                }
                break;
            case (char)INSTRUCTION_TYPE::BLT: // Branch if less than
                if ( getRegistry(currentINSTR.b[1])[0] < getRegistry(currentINSTR.b[2])[0] ){
                    PC++;
                } else {
                    PC += 2;
                }
                break;
            case (char)INSTRUCTION_TYPE::SBIX: // Store byte immediate value
                RAM[currentINSTR.b[2]] = getRegistry(currentINSTR.b[1])[0];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::LBIX: // Load byte immediate value
                getRegistry(currentINSTR.b[1])[0] = RAM[currentINSTR.b[2]];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::JRT: // JUMP to RET value
                PC = RET;
                break;
            case (char)INSTRUCTION_TYPE::PUSH:{ // PUSH Register to stack
                unsigned char * specialREG = getSpecialRegistry(currentINSTR.b[1]);

                if ( specialREG == 0 ) { // If not a special registry
                    stackPointer -= 1;
                    if ( stackPointer < 0 ){ std::cout << "STACK OVERFLOW" << std::endl; }
                    unsigned char * gpREG = getRegistry(currentINSTR.b[1]);
                    RAM[stackPointer] = gpREG[0];
                } else { // If a special registry
                    stackPointer -= 2;
                    if ( stackPointer < 0 ){ std::cout << "STACK OVERFLOW" << std::endl; }
                    RAM[stackPointer + 1] = specialREG[0];
                    RAM[stackPointer] = specialREG[1];
                }

                updateSP();
                PC++;
                break;
            }
            case (char)INSTRUCTION_TYPE::POP:{ // POP Register from stack
                unsigned char * specialREG = getSpecialRegistry(currentINSTR.b[1]);

                if ( specialREG == 0 ) { // If not a special registry
                    unsigned char * gpREG = getRegistry(currentINSTR.b[1]);
                    gpREG[0] = RAM[stackPointer];

                    stackPointer += 1;
                    if ( stackPointer > EMULATOR_RAM ){ std::cout << "STACK UNDERFLOW" << std::endl; }
                } else { // If a special registry

                    specialREG[0] = RAM[stackPointer + 1];
                    specialREG[1] = RAM[stackPointer];

                    stackPointer += 2;
                    if ( stackPointer > EMULATOR_RAM ){ std::cout << "STACK UNDERFLOW" << std::endl; }
                }

                updateSP();
                PC++;
                break;
            }
            case (char)INSTRUCTION_TYPE::RRA: // Load from ROM into RA
                // RRA REG, VALUE -> VALUE=2 bytes
                // INSTRCODE REG VALUE_HIGH VALUE_LOW
                label.BYTE[1] = currentINSTR.b[2];
                label.BYTE[0] = currentINSTR.b[3];
                RA = ROM[4 + label.SHORT + (unsigned short)getRegistry(currentINSTR.b[1])[0]];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::RRB: // Load from ROM into RB
                label.BYTE[1] = currentINSTR.b[2];
                label.BYTE[0] = currentINSTR.b[3];
                RB = ROM[4 + label.SHORT + (unsigned short)getRegistry(currentINSTR.b[1])[0]];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::RRC: // Load from ROM into RC
                label.BYTE[1] = currentINSTR.b[2];
                label.BYTE[0] = currentINSTR.b[3];
                RC = ROM[4 + label.SHORT + (unsigned short)getRegistry(currentINSTR.b[1])[0]];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::RRD: // Load from ROM into RD
                label.BYTE[1] = currentINSTR.b[2];
                label.BYTE[0] = currentINSTR.b[3];
                RD = ROM[4 + label.SHORT + (unsigned short)getRegistry(currentINSTR.b[1])[0]];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::RRE: // Load from ROM into RE
                label.BYTE[1] = currentINSTR.b[2];
                label.BYTE[0] = currentINSTR.b[3];
                RE = ROM[4 + label.SHORT + (unsigned short)getRegistry(currentINSTR.b[1])[0]];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::RRF: // Load from ROM into RF
                label.BYTE[1] = currentINSTR.b[2];
                label.BYTE[0] = currentINSTR.b[3];
                std::cout<< "RRF " << label.SHORT << " " << (unsigned short)getRegistry(currentINSTR.b[1])[0] << std::endl;
                RF = ROM[4 + label.SHORT + (unsigned short)getRegistry(currentINSTR.b[1])[0]];
                PC++;
                break;
            case (char)INSTRUCTION_TYPE::XOR: // XOR operation on three registers
                // std::cout << "XOR : " << (int)getRegistry(currentINSTR.b[2])[0] << ' ' << (int)getRegistry(currentINSTR.b[3])[0] << std::endl;
                getRegistry(currentINSTR.b[1])[0] = getRegistry(currentINSTR.b[2])[0] ^
                                                    getRegistry(currentINSTR.b[3])[0];
                PC++;
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

sf::Color c0 = sf::Color(16,16,16);// 0111
sf::Color c1 = sf::Color(32,32,32);// 1000
sf::Color c2 = sf::Color(48,48,48);// 1001
sf::Color c3 = sf::Color(64,64,64);// 1010
sf::Color c4 = sf::Color(80,80,80);// 1011
sf::Color c5 = sf::Color(96,96,96);// 1100
sf::Color c6 = sf::Color(112,112,112);// 1101
sf::Color c7 = sf::Color(128,128,128);// 1110

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


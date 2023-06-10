#pragma once
#include <cstdint>
#include <array>
#include <stack>
#include "../ControlHandler.h"
#include <string>

class CPU {
    public:
        ControlHandler control_handler;

        const std::uint16_t PROGRAM_START;
       
        std::array<std::uint8_t, 4096> ram;
        std::array<std::uint8_t, 16> v; 
        std::stack<std::uint16_t> stack; 
        std::array<std::array<bool,64>,32> screen; 
        std::uint16_t i;
        std::uint8_t sound;
        std::uint8_t delay;
        std::uint16_t pc;
        std::uint8_t sp;

        CPU(ControlHandler control_handler);

        void execute();
        bool readInput(std::uint8_t key);
        bool isKeyPressed();
        void clearScreen();
        void loadRam(std::string string);
};
        

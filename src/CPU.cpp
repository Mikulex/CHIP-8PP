#include <cstdint>
#include <array>
#include <ios>
#include <iostream>
#include <fstream>
#include <ostream>
#include <stack>
#include <system_error>
#include "CPU.h"
#include "../ControlHandler.h"


CPU::CPU(ControlHandler control_handler) :
	PROGRAM_START{ 0x200 },
	pc{ PROGRAM_START },
	control_handler{ control_handler },
	screen{ {{0}} },
	v{ 0 },
	ram{ 0 },
	i{ 0 },
	sound{ 0 },
	delay{ 0 }
	{
		// 0, 1
		ram[0] = 0xF0;
		ram[1] = 0x90;
		ram[2] = 0x90;
		ram[3] = 0x90;
		ram[4] = 0xF0;
		ram[5] = 0x20;
		ram[6] = 0x60;
		ram[7] = 0x20;
		ram[8] = 0x20;
		ram[9] = 0x70;

		// 2, 3
		ram[10] = 0xF0;
		ram[11] = 0x10;
		ram[12] = 0xF0;
		ram[13] = 0x80;
		ram[14] = 0xF0;
		ram[15] = 0xF0;
		ram[16] = 0x10;
		ram[17] = 0xF0;
		ram[18] = 0x10;
		ram[19] = 0xF0;

		// 4, 5
		ram[20] = 0x90;
		ram[21] = 0x90;
		ram[22] = 0xF0;
		ram[23] = 0x10;
		ram[24] = 0x10;
		ram[25] = 0xF0;
		ram[26] = 0x80;
		ram[27] = 0xF0;
		ram[28] = 0x10;
		ram[29] = 0xF0;

		// 6, 7
		ram[30] = 0xF0;
		ram[31] = 0x80;
		ram[32] = 0xF0;
		ram[33] = 0x90;
		ram[34] = 0xF0;
		ram[35] = 0xF0;
		ram[36] = 0x10;
		ram[37] = 0x20;
		ram[38] = 0x40;
		ram[39] = 0x40;

		// 8, 9
		ram[40] = 0xF0;
		ram[41] = 0x90;
		ram[42] = 0xF0;
		ram[43] = 0x90;
		ram[44] = 0xF0;
		ram[45] = 0xF0;
		ram[46] = 0x90;
		ram[47] = 0xF0;
		ram[48] = 0x10;
		ram[49] = 0xF0;

		// A, B
		ram[50] = 0xF0;
		ram[51] = 0x90;
		ram[52] = 0xF0;
		ram[53] = 0x90;
		ram[54] = 0x90;
		ram[55] = 0xE0;
		ram[56] = 0x90;
		ram[57] = 0xE0;
		ram[58] = 0x90;
		ram[59] = 0xE0;

		// C, D
		ram[60] = 0xF0;
		ram[61] = 0x80;
		ram[62] = 0x80;
		ram[63] = 0x80;
		ram[64] = 0xF0;
		ram[65] = 0xE0;
		ram[66] = 0x90;
		ram[67] = 0x90;
		ram[68] = 0x90;
		ram[69] = 0xE0;

		// E, F
		ram[70] = 0xF0;
		ram[71] = 0x80;
		ram[72] = 0xF0;
		ram[73] = 0x80;
		ram[74] = 0xF0;
		ram[75] = 0xF0;
		ram[76] = 0x80;
		ram[77] = 0xF0;
		ram[78] = 0x80;
		ram[79] = 0x80;
		std::cout << "finished loading fonts" << std::endl;
		std::srand(std::time(nullptr)); 
	}

void CPU::execute() {
	std::uint16_t instruction = (this->ram[this->pc] << 8) | (this->ram[this->pc + 1] & 0x00FF);
	this->pc += 2;

	switch (instruction & 0xF000) {
		case 0x0000:
			if (instruction == 0x00E0) {
				this->clearScreen();
			}
			else if (instruction == 0x00EE) { // return
				this->pc = this->stack.top();
				this->stack.pop();
			}
			else {
				throw "Unknown instruction with opcode 0x0";
			}
			break;
		case 0x1000: // jump
			this->pc = instruction & 0x0FFF;
			break;
		case 0x2000: // call subroutine
			stack.push(this->pc);
			this->pc = (instruction & 0xFFF);
			break;
		case 0x3000: { // skip if equal 
			std::uint16_t x = (instruction & 0x0F00) >> 8;
			if (this->v[x] == (instruction & 0x00FF)) {
				this->pc += 2;
			}
			break;
		}
		case 0x4000: { // skip if not equal 
			std::uint16_t x = (instruction & 0x0F00) >> 8;
			if (this->v[x] != (instruction & 0x00FF)) {
				this->pc += 2;
			}
			break;
		}
		case 0x5000: { // skip if equal
			std::uint16_t x = (instruction & 0x0F00) >> 8;
			std::uint16_t y = (instruction & 0x00F0) >> 4;
			if (this->v[x] == this->v[y]) {
				this->pc += 2;
			}
			break;
		}
		case 0x6000: { // set v
			std::uint16_t x = (instruction & 0x0F00) >> 8;
			this->v[x] = (instruction & 0x00FF);
			break;
		}
		case 0x7000: { // add to v		
			std::uint16_t x = (instruction & 0x0F00) >> 8;
			std::uint16_t newVal = this->v[x] + (instruction & 0x00FF);
			this->v[x] = newVal;
			break;
		}
		case 0x8000: {
			std::uint16_t x = (instruction & 0x0F00) >> 8;
			std::uint16_t y = (instruction & 0x00F0) >> 4;
			switch (instruction & 0x000F) {
				case 0x0: // load
					this->v[x] = this->v[y];
					break;
				case 0x1: // or
					this->v[x] = this->v[x] | this->v[y];
					break;
				case 0x2: // and
					this->v[x] = this->v[x] & this->v[y];
					break;
				case 0x3: // xor
					this->v[x] = this->v[x] ^ this->v[y];
					break;
				case 0x4: {// add
					std::uint16_t newVal = this->v[x] + this->v[y];
					this->v[x] = newVal;
					if (newVal > 0xFF) {
						this->v[0xF] = 1;
					}
					else {
						this->v[0xF] = 0;
					}
					break;
				}
				case 0x5: {// sub
					bool borrowed = this->v[x] > this->v[y];
					this->v[x] = this->v[x] - this->v[y];
					this->v[0xF] = borrowed ? 1 : 0;
					break;
				}
				case 0x6: {// div
					std::uint8_t shifted_bit = this->v[x] & 1;
					this->v[x] = this->v[x] >> 1;
					this->v[0xF] = shifted_bit;
					break;
				}
				case 0x7: {// subn
					bool borrowed = this->v[x] < this->v[y];
					this->v[x] = this->v[y] - this->v[x];
					this->v[0xF] = borrowed ? 1 : 0;
					break;
				}
				case 0xE: {// mul
					std::uint8_t shifted_bit = (this->v[x] & 0b1000'0000) >> 7;	
					this->v[x] = this->v[x] << 1;
					this->v[0xF] = shifted_bit;
					break;
				}

			}
			break;
		}
		case 0x9000: { // skip if not equal
			std::uint16_t x = (instruction & 0x0F00) >> 8;
			std::uint16_t y = (instruction & 0x00F0) >> 4;
			if (this->v[x] != this->v[y]) {
				this->pc += 2;
			}
			break;
		}
		case 0xA000: // set i
			this->i = (instruction & 0x0FFF);
			break;
		case 0xB000: // jump with offset
			this->pc = (instruction & 0x0FFF) + this->v[0];
			break;
		case 0xC000: {
			std::uint16_t x = (instruction & 0x0F00) >> 8;
			this->v[x] = (std::rand() % 255) & (instruction & 0x00FF);
			break;
		}
		case 0xD000: { // draw
			std::uint8_t x = (instruction & 0x0F00) >> 8;
			std::uint8_t y = (instruction & 0x00F0) >> 4;
			std::uint8_t n = (instruction & 0x000F);

			this->v[0xF] = 0;

			for (int k = 0; k < n; k++) {
				std::uint8_t row = (this->v[y] + k);
				if (row > 32 && k == 0) {
					break;
				}
				row %= 32;
				std::uint8_t sprite = this->ram[this->i + k];

				for (std::uint8_t j = 0; j < 8; j++) {
					std::uint8_t column = (this->v[x] + j);
					if (column > 64 && j == 0) {
						break;
					}
					column %= 64;
					bool isOn = this->screen[row][column];
					bool newVal = (sprite & (0x80 >> j)) != 0;
					if (isOn && newVal) { // got turned off -> collision
						this->v[0xF] = 1;
					}
					this->screen[row][column] = isOn != newVal;
				}
			}
			break;
		}
		case 0xE000: { // skip if key (not) pressed
			std::uint8_t x = (instruction & 0x0F00) >> 8;
			switch (instruction & 0x00FF) {
				case 0x009E: 
					if (readInput(this->v[x])) {
						this->pc += 2;
					}
					break;
				case 0x00A1:
					if (!readInput(this->v[x])) {
						this->pc += 2;
					}
					break;
			}
			break;
		}
		case 0xF000: {
			std::uint8_t x = (instruction & 0x0F00) >> 8;
			switch (instruction & 0x00FF) {
				case 0x0007: { // set register to delay value
					this->v[x] = this->delay;
					break;
				}
				case 0x000A: { // wait for key press
					std::uint8_t pressed_key;
					bool pressed = isKeyPressed(pressed_key);
					if(pressed) {
						this->v[x] = pressed_key;
					}
					else { 
						this->pc -= 2; 
					}
					break;
				}
				case 0x0015: { // set delay to register value
					this->delay = this->v[x];
					break;
				}
				case 0x0018: { // set sound to register value
					this->sound= this->v[x];
					break;
				}
				case 0x001E: { // add register value to i
					this->i = this->i + this->v[x];
					break;
				}
				case 0x0029: { // load sprite
					this->i = this->ram[v[x] * 5];
					break;
				}
				case 0x0033: { // do bcd conversion
					std::uint8_t val = this->v[x];
					this->ram[this->i] = val / 100;
					this->ram[this->i + 1] = (val % 100) / 10;
					this->ram[this->i + 2] = val % 10;
					break;
				}
				case 0x0055: { // load registers into ram
					std::uint16_t current = this->i;
					for (int k = 0; k <= x; k++) {
						this->ram[current] = this->v[k];
						current++;
					}
					break;
				}
				case 0x0065: { // load ram into registers
					std::uint16_t current = this->i;
					for (int k = 0; k <= x; k++) {
						this->v[k] = this->ram[current];
						current++;
					}
					break;
				}
			}
			break;
		}	
	}
}

bool CPU::readInput(std::uint8_t key) {
	return control_handler.keyPressed(key);
}

bool CPU::isKeyPressed(std::uint8_t& pressed_key) {
	return control_handler.isKeyPressed(pressed_key);
}

void CPU::clearScreen() {
	for (int y = 0; y < this->screen.size(); y++) {
		for (int x = 0; x < this->screen[y].size(); x++) {
			screen[y][x] = false;
		}
	}
}

void CPU::loadRam(std::string string) {
	std::ifstream infile(string, std::ios::binary | std::ios::in | std::ios::ate);
	std::streamsize size = infile.tellg();

	infile.seekg(0, std::ios::beg);

	infile.read(reinterpret_cast<char*>(this->ram.data() + (sizeof(char) * this->PROGRAM_START)), size);
	std::cout << "finished loading into ram" << std::endl;
}

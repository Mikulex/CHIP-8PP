#include "CPU.h"
#include <iostream>

void drawScreen(CPU& cpu){
    std::cout << "drawing screen" << std::endl;
    for (int y = 0; y < cpu.screen.size(); y++) {
        std::cout << "ROW NUMBER " << y << '\t';
        for (int x = 0; x < cpu.screen[y].size(); x++) {
            if (cpu.screen[y][x]) {
                std::cout << '#';
            } else {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
    }

}

int main() {
    CPU cpu{};
    cpu.loadRam();

    long long i = 0;
    while (true) {
        cpu.execute();
        
        if (i == 50000000) {
            drawScreen(cpu);
            i = 0;
        }
        i++;
    }

    return 0;
}


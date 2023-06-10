#include "CPU.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../ControlHandler.h"


void drawScreen(CPU& cpu, sf::RenderWindow& window, sf::Image& image) {
    for (int y = 0; y < cpu.screen.size(); y++) {
        for (int x = 0; x < cpu.screen[y].size(); x++) {
            if (cpu.screen[y][x]) {
                image.setPixel(x,y,sf::Color::Green);
            }
            else {
                image.setPixel(x, y, sf::Color::Black);
            }
        } 
    }
}

std::string chooseRom() {
    std::cout << "enter filename" << std::endl;
    std::string file;
    std::getline(std::cin, file);
    return file;
}

int main() {
    const int scale = 8;
    ControlHandler control_handler{ sf::Keyboard{} };
    CPU cpu{control_handler};
    
    sf::RenderWindow window(sf::VideoMode(64 * scale, 32 * scale), "CHIP-8PP Emulator");    
    window.setFramerateLimit(60);
    
    sf::Image image;
    image.create(64 * scale, 32 * scale);
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(scale, scale));

    std::string rom = chooseRom();
    cpu.loadRam(rom);

    while (window.isOpen()) {
        cpu.execute();

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear(sf::Color::Black);
        drawScreen(cpu, window, image);
        texture.update(image);
        
        window.draw(sprite);
        window.display();
    }
    return 0;
}


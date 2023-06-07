#include "CPU.h"
#include <iostream>
#include <SFML/Graphics.hpp>


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

int main() {
    int scale = 8;
    sf::RenderWindow window(sf::VideoMode(64 * scale, 32 * scale), "CHIP-8PP Emulator");
    CPU cpu{};
    cpu.loadRam();

    window.setFramerateLimit(60);
    sf::Image image;
    image.create(64 * scale, 32 * scale);
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(sf::Vector2f(scale, scale));

    while (window.isOpen())
    {
        cpu.execute();

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
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


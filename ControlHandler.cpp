#include "ControlHandler.h"

ControlHandler::ControlHandler(sf::Keyboard state) : state {state}, key_map {
    { 0x1, sf::Keyboard::Key::Num1 },
    { 0x2, sf::Keyboard::Key::Num2 },
    { 0x3, sf::Keyboard::Key::Num3 },
    { 0x4, sf::Keyboard::Key::Q },
    { 0x5, sf::Keyboard::Key::W },
    { 0x6, sf::Keyboard::Key::E },
    { 0x7, sf::Keyboard::Key::A },
    { 0x8, sf::Keyboard::Key::S },
    { 0x9, sf::Keyboard::Key::D },
    { 0x0, sf::Keyboard::Key::X },
    { 0xA, sf::Keyboard::Key::Z },
    { 0xB, sf::Keyboard::Key::C },
    { 0xC, sf::Keyboard::Key::Num4 },
    { 0xD, sf::Keyboard::Key::R },
    { 0xE, sf::Keyboard::Key::F },
    { 0xF, sf::Keyboard::Key::V }
} {}

bool ControlHandler::keyPressed(std::uint8_t key) {
    auto key_to_check = key_map.at(key);
    return key_to_check != NULL && state.isKeyPressed(key_to_check);
}

bool ControlHandler::isKeyPressed() {
    for (auto pair: key_map) {
        if (state.isKeyPressed(pair.second)) {
            return true;
        }
    }
    return false;
}

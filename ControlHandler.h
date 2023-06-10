#pragma once
#include<unordered_map>
#include <SFML/Window/Keyboard.hpp>

class ControlHandler {
public: 
	std::unordered_map<std::uint8_t, sf::Keyboard::Key> key_map;
	sf::Keyboard state;

	ControlHandler(sf::Keyboard state);
	bool keyPressed(std::uint8_t);
	bool isKeyPressed();
};


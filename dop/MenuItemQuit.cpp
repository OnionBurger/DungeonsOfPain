#include "MenuItemQuit.h"

MenuItemQuit::MenuItemQuit(sf::Vector2f startCoords) : MenuItem(startCoords, "quit.png") {
}

void MenuItemQuit::quit() {
	endGame();
}

void MenuItemQuit::onClick() {
	if (getMouseButton() == sf::Mouse::Left) quit();
}
#include "MenuItemStart.h"
#include "RoomChooseAvatar.h"

MenuItemStart::MenuItemStart(sf::Vector2f startCoords) : MenuItem(startCoords, "start_game.png") {
}

void MenuItemStart::start() {
	changeRoom(new RoomChooseAvatar());
}

void MenuItemStart::onClick() {
	if (getMouseButton() == sf::Mouse::Left) start();
}
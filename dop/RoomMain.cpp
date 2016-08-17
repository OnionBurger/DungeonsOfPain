#include "RoomMain.h"
#include "MenuItemStart.h"
#include "MenuItemQuit.h"
#include <Morph\Flow.h>
#include "GlobalTextureCont.h"

void RoomMain::onEnter() {
	sf::Vector2u screenSize = getFlow()->getWindow()->getSize();

	title.setTexture(catchMenuTexture("title.png"), true);
	title.setOrigin(title.getLocalBounds().width / 2.f, title.getLocalBounds().height / 2.f);
	title.setPosition(screenSize.x / 2, screenSize.y / 4);

	addObject(menuItemStart = new MenuItemStart(sf::Vector2f(screenSize.x / 2, 2 * screenSize.y / 4)));
	addObject(menuItemQuit = new MenuItemQuit(sf::Vector2f(screenSize.x / 2, 3 * screenSize.y / 4)));
}

void RoomMain::postEvent(const sf::Event &e) {
	if (e.type == sf::Event::KeyPressed) {
		if (e.key.code == sf::Keyboard::Return || e.key.code == sf::Keyboard::Space || e.key.code == sf::Keyboard::LControl || e.key.code == sf::Keyboard::RControl)
			menuItemStart->start();
		else if (e.key.code == sf::Keyboard::Escape)
			menuItemQuit->quit();
	}
}

void RoomMain::drawSelf(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(title, states);
}
#include "Door.h"
#include <Morph\Looks\LookSprite.h>
#include "GlobalTextureCont.h"

Door::Door(const sf::Vector2f &offset, const std::string &look) : morph::Object(offset, 10) {
	revealed = false;
	seen = false;

	setRendered(false);
	setLook(new morph::LookSprite(catchDoorTexture(look), true));
}

void Door::setRevealed(bool r) {
	if (revealed == r) return;

	revealed = r;

	if (revealed) {
		seen = true;
		setRendered(true);
		((morph::LookSprite*)(getLook()))->sprite.setColor(sf::Color(255, 255, 255));
	} else {
		((morph::LookSprite*)(getLook()))->sprite.setColor(sf::Color(255/2, 255/2, 255/2));
	}
}
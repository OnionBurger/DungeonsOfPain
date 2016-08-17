#include "Wall.h"
#include <Morph\Looks\LookSprite.h>
#include "GlobalTextureCont.h"

Wall::Wall(const sf::Vector2f &offset, const std::string &look) : morph::Object(offset, 10) {
	revealed = false;
	seen = false;

	setRendered(false);
	setLook(new morph::LookSprite(catchWallTexture(look), true));
}

void Wall::setRevealed(bool r) {
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
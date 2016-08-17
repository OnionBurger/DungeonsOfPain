#include "Stairs.h"
#include <Morph\Looks\LookSprite.h>
#include "GlobalTextureCont.h"

Stairs::Stairs(const sf::Vector2f &offset) : morph::Object(offset, 15) {
	revealed = false;
	seen = false;

	setRendered(false);
	setLook(new morph::LookSprite(catchStairsTexture("stairs.png"), true));
}

void Stairs::setRevealed(bool r) {
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
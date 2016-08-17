#include "MenuItem.h"
#include <Morph\Looks\LookSprite.h>
#include <Morph\CollMasks\CollRect.h>
#include "GlobalTextureCont.h"

MenuItem::MenuItem(sf::Vector2f startCoords, const std::string &textureName) 
	: morph::ObjButton(startCoords, -10) {
	morph::LookSprite *spr = new morph::LookSprite(catchMenuTexture(textureName), true);
	
	sf::FloatRect bounds = spr->sprite.getLocalBounds();

	spr->setOrigin(sf::Vector2f((unsigned)bounds.width / 2, (unsigned)bounds.height / 2));
	setLook(spr);

	morph::CollisionMask *rect = new morph::CollRect(bounds.width, bounds.height);
	rect->setOrigin(spr->getOrigin());
	setCollMask(rect);
}
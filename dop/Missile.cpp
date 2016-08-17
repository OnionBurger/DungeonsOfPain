#include "Missile.h"
#include "Map.h"
#include "Target.h"
#include <Morph\Looks\LookSprite.h>
#include "GlobalTextureCont.h"
#include <Morph\Room.h>

Missile::Missile(Map *M, Target *from, Target *to, std::string textureFileName) 
	: morph::Object(-5) {
	if (from) {
		setPosition(from->getCenter());
	}
	
	if (to == 0) {
		destination = getPosition();
	} else {
		destination = sf::Vector2f(to->getCenter());
	}

	morph::LookSprite *spr = 
		new morph::LookSprite(catchFxTexture(textureFileName), true);

	spr->setOrigin(sf::Vector2f(spr->sprite.getLocalBounds().width / 2, 
		spr->sprite.getLocalBounds().height / 2));

	setLook(spr);

	float intensity = 6.f;
	float dx = destination.x - getPosition().x;
	float dy = destination.y - getPosition().y;
	if (dx == 0 && dy == 0) {
		speed = sf::Vector2f(0.f, 0.f);
	}
	else {
		float denominator = sqrt(dx * dx + dy * dy);
		speed = intensity * sf::Vector2f(dx / denominator, dy / denominator);

		float pi = acos(-1.f); // apparently, standard C++ has no definition for pi
		float rot = acos(dx / denominator) * 180 / pi;
		if (dy < 0) rot = -rot;
		spr->setRotation(rot);
	}
}

void Missile::onTick() {
	static const float epsilonTolerance = 0.01f;

	if (abs(getPosition().x - destination.x) < epsilonTolerance 
		&& abs(getPosition().y - destination.y) < epsilonTolerance) {
		getRoom()->destroyObject(this);
		return;
	}

	sf::Vector2f newPos;

	if (speed.x > 0) {
		if (getPosition().x + speed.x <= destination.x) newPos.x = getPosition().x + speed.x;
		else newPos.x = destination.x;
	} else if (speed.x < 0) {
		if (getPosition().x + speed.x >= destination.x) newPos.x = getPosition().x + speed.x;
		else newPos.x = destination.x;
	} else {
		newPos.x = getPosition().x;
	}

	if (speed.y > 0) {
		if (getPosition().y + speed.y <= destination.y) newPos.y = getPosition().y + speed.y;
		else newPos.y = destination.y;
	} else if (speed.y < 0) {
		if (getPosition().y + speed.y >= destination.y) newPos.y = getPosition().y + speed.y;
		else newPos.y = destination.y;
	} else {
		newPos.y = getPosition().y;
	}

	setPosition(newPos);
}
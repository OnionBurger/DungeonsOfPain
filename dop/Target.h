#ifndef _Target_h_
#define _Target_h_

#include <SFML\System\Vector2.hpp>

class Effect;

class Target {
public:

	virtual sf::Vector2f getCenter() const =0;

	virtual void destroy(const Effect *E) {}
	virtual void heal(const Effect *E) {}
	virtual void mana(const Effect *E) {}
	virtual void hurt(const Effect *E) {}
	virtual void missile(const Effect *E) {}
	virtual void buff(const Effect *E) {}

	virtual ~Target() {}
};

#endif
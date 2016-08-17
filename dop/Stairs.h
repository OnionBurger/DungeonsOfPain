#ifndef _Stairs_h_
#define _Stairs_h_

#include <Morph\Object.h>

class Stairs : public morph::Object {
	bool revealed;
	bool seen;

public:
	Stairs(const sf::Vector2f &offset);

	void setRevealed(bool r);
	bool isRevealed() const { return revealed; }
};

#endif
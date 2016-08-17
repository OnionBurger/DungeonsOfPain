#ifndef _Wall_h_
#define _Wall_h_

#include <Morph\Object.h>

class Wall : public morph::Object {
	bool revealed;
	bool seen;

public:
	Wall(const sf::Vector2f &offset, const std::string &look);

	void setRevealed(bool r);
	bool isRevealed() const { return revealed; }
};

#endif
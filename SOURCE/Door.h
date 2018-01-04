#ifndef _Door_h_
#define _Door_h_

#include <Morph\Object.h>

class Door : public morph::Object {
	bool revealed;
	bool seen;

public:
	Door(const sf::Vector2f &offset, const std::string &look);

	void setRevealed(bool r);
	bool isRevealed() const { return revealed; }
};

#endif
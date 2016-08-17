#ifndef _Missile_h_
#define _Missile_h_

#include <Morph\Object.h>
#include <string>

class Map;
class Target;

class Missile : public morph::Object {
	sf::Vector2f speed, destination;

public:
	Missile(Map *M, Target *from, Target *to, std::string textureFileName);

	void onTick();
};

#endif
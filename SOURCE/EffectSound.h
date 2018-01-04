#ifndef _Effect_Sound_h_
#define _Effect_Sound_h_

#include "Effect.h"
#include <SFML\Audio.hpp>

class EffectSound : public Effect {
	sf::Sound sound;
	std::string soundFileName;

public:
	EffectSound(const pugi::xml_node &node);

	void run(Target *T);
};

#endif
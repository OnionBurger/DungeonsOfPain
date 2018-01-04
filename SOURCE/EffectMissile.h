#ifndef _Effect_Missile_h_
#define _Effect_Missile_h_

#include "Effect.h"

class EffectMissile : public Effect {
	std::string from, missile;

	Target *src;

public:
	EffectMissile(const pugi::xml_node &node);

	void run(WithTriggers *activator);
	void run(Target *T);

	Target* getSource() { return src; }

	const std::string& getFrom() const { return from; }
	const std::string& getMissile() const { return missile; }
};

#endif
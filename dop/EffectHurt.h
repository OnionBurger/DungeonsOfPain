#ifndef _Effect_Hurt_h_
#define _Effect_Hurt_h_

#include "Effect.h"

class EffectHurt : public Effect {
	int amount, currMagicStat;
	float magicCoef;

public:
	EffectHurt(const pugi::xml_node &node);
	
	void run(WithTriggers *activator);
	void run(Target *T);

	int getAmount() const;
};

#endif
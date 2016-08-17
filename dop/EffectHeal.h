#ifndef _Effect_Heal_h_
#define _Effect_Heal_h_

#include "Effect.h"

class EffectHeal : public Effect {
	int amount;

public:
	EffectHeal(const pugi::xml_node &node);

	void run(Target *T);

	int getAmount() const { return amount; }
};

#endif
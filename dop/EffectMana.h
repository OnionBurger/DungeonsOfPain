#ifndef _Effect_Mana_h_
#define _Effect_Mana_h_

#include "Effect.h"

class EffectMana : public Effect {
	int amount;

public:
	EffectMana(const pugi::xml_node &node);

	void run(Target *T);

	int getAmount() const { return amount; }
};

#endif
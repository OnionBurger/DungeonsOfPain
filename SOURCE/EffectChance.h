#ifndef _Effect_Chance_h_
#define _Effect_Chance_h_

#include "Effect.h"

class StorageEffects;

class EffectChance : public Effect {
public:
	Effect *eff;

	float chance;

public:
	EffectChance(StorageEffects *S, const pugi::xml_node &node);
	
	void run(WithTriggers *activator);
	void run(Target *T);
};

#endif
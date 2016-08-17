#ifndef _Effect_Combine_h_
#define _Effect_Combine_h_

#include "Effect.h"
#include <vector>

class StorageEffects;

class EffectCombine : public Effect {
public:
	typedef std::pair<Effect*, std::string> EffectAndTarget;

	std::vector<EffectAndTarget> refs;

public:
	EffectCombine(StorageEffects *S, const pugi::xml_node &node);

	void run(WithTriggers *activator);
	void run(Target *T);
};

#endif
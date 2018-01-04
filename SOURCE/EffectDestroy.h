#ifndef _Effect_Destroy_h_
#define _Effect_Destroy_h_

#include "Effect.h"

class EffectDestroy : public Effect {
public:
	EffectDestroy(const pugi::xml_node &node);

	void run(Target *T);
};

#endif
#ifndef _Effect_Buff_h_
#define _Effect_Buff_h_

#include "Effect.h"

class EffectBuff : public Effect {
	std::string buff;

public:
	EffectBuff(const pugi::xml_node &node);

	void run(Target *T);

	const std::string& getBuffHandle() const { return buff; }
};

#endif
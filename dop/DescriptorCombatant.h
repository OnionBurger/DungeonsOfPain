#ifndef _Descriptor_Combatant_h_
#define _Descriptor_Combatant_h_

#include "DescriptorWithStats.h"
#include "DescriptorWithTriggers.h"

class DescriptorCombatant : public DescriptorWithStats, public DescriptorWithTriggers {
public:
	DescriptorCombatant(const pugi::xml_node &node);
};

#endif
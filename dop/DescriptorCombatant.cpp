#include "DescriptorCombatant.h"

DescriptorCombatant::DescriptorCombatant(const pugi::xml_node &node) : 
	DescriptorWithStats(node), 
	DescriptorWithTriggers(node) {
}
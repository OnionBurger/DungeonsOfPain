#ifndef _Descriptor_Enemy_h_
#define _Descriptor_Enemy_h_

#include "DescriptorCombatant.h"

class DescriptorEnemy : public DescriptorCombatant {
public:
	std::string handle, name;
	unsigned genWeight;
	int diff;
	std::string textureFileName;
	std::string missileTextureFileName;

private:
	void apply(const pugi::xml_node &node);

public:
	DescriptorEnemy(const pugi::xml_node &node);
	
	bool diffInRange(int min, int max) const;
};

#endif
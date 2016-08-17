#ifndef _Descriptor_Avatar_h_
#define _Descriptor_Avatar_h_

#include "DescriptorCombatant.h"

class DescriptorAvatar : public DescriptorCombatant {
public:
	std::string handle, name, description;
	std::string textureFileName;

	std::vector<std::string> startingItems;

private:
	void apply(const pugi::xml_node &node);

public:
	DescriptorAvatar(const pugi::xml_node &node);
};

#endif
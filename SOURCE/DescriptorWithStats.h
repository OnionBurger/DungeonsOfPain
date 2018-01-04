#ifndef _Descriptor_With_Stats_h_
#define _Descriptor_With_Stats_h_

#include <string>
#include "pugixml.hpp"
#include "WithStats.h"

class DescriptorWithStats {
public:
	std::vector<WithStats::StatVal> stats;
	bool ranged;
	std::string missileTextureFileName;
	
	bool immobile;

private:
	void setDefaultValues();

	void apply(const pugi::xml_node &node);

public:
	DescriptorWithStats();
	DescriptorWithStats(const pugi::xml_node &node);
};

#endif
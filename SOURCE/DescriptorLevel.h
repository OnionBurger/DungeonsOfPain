#ifndef _Descriptor_Level_h_
#define _Descriptor_Level_h_

#include <string>
#include "pugixml.hpp"

class DescriptorLevel {
public:
	int minScore, maxScore;
	int minDiff, maxDiff;

	std::string door, floor, wall;

private:
	void setDefaultValues();

	void apply(const pugi::xml_node &node);

public:
	DescriptorLevel(const pugi::xml_node &node);
};

#endif
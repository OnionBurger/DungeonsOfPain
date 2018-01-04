#ifndef _Descriptor_With_Triggers_h_
#define _Descriptor_With_Triggers_h_

#include <string>
#include "pugixml.hpp"
#include "Trigger.h"
#include <vector>

class DescriptorWithTriggers {
public:
	std::vector<Trigger> effects;

private:
	void apply(const pugi::xml_node &node);
	
protected:
	DescriptorWithTriggers();

public:
	DescriptorWithTriggers(const pugi::xml_node &node);

	virtual ~DescriptorWithTriggers() {}
};

#endif
#ifndef _Descriptor_Buff_h_
#define _Descriptor_Buff_h_

#include "DescriptorWithTriggers.h"
#include "DescriptorWithStats.h"
#include <string>
#include <vector>

class DescriptorBuff : public DescriptorWithStats, public DescriptorWithTriggers {
public:
	std::string handle, name;

	struct Duration {
		std::string type;
		unsigned val;

		Duration(const pugi::xml_node &node);
	};

	std::vector<Duration> durations;

private:
	void setDefaultValues();

	void applyDurationChild(const pugi::xml_node &node);
	void apply(const pugi::xml_node &node);

public:
	DescriptorBuff(const pugi::xml_node &node);
};

#endif
#ifndef _Effect_h_
#define _Effect_h_

#include <string>
#include "pugixml.hpp"

class Target;
class WithTriggers;

class Effect {
protected:
	std::string handle, type, target;

private:
	void readBasicInfo(const pugi::xml_node &node);

public:
	Effect(const pugi::xml_node &node);

	virtual void run(WithTriggers *activator);
	virtual void run(Target *T) =0;

	const std::string& getHandle() const { return handle; }

	virtual ~Effect() {}
};

#endif
#ifndef _Trigger_h_
#define _Trigger_h_

#include <string>

class Effect;

class Trigger {
public:
	enum Trigg {
		T_UNDEFINED,
		T_USE, 
		T_STEP, 
		T_HIT, 
		T_CRIT, 
		T_MISS, 
		T_STRUCK, 
		T_AVOID, 
		T_DESTROY, 
		T_PICKUP, 
		T_DROP, 
		T_EQUIP, 
		T_UNEQUIP
	};

	static Trigg stringToTrigg(const std::string &str);

public:
	Trigg cause;
	std::string handle;
	Effect *effect;
	std::string target;
};

#endif
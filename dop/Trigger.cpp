#include "Trigger.h"

Trigger::Trigg Trigger::stringToTrigg(const std::string &str) {
	if (str == "use") return T_USE;
	else if (str == "step") return T_STEP;
	else if (str == "hit") return T_HIT;
	else if (str == "crit") return T_CRIT;
	else if (str == "miss") return T_MISS;
	else if (str == "struck") return T_STRUCK;
	else if (str == "avoid") return T_AVOID;
	else if (str == "destroy") return T_DESTROY;
	else if (str == "pickup") return T_PICKUP;
	else if (str == "drop") return T_DROP;
	else if (str == "equip") return T_EQUIP;
	else if (str == "unequip") return T_UNEQUIP;

	return T_UNDEFINED;
}
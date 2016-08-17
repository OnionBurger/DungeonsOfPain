#include "WithStats.h"
#include "DescriptorWithStats.h"

std::string WithStats::statToString(Stat S) {
	switch (S) {
	case S_MAX_HP: return "max_hp";
	case S_MAX_MP: return "max_mp";
	case S_STR: return "str";
	case S_ACC: return "acc";
	case S_P_RES: return "p_res";
	case S_M_RES: return "m_res";
	case S_DXT: return "dxt";
	case S_DED: return "ded";
	case S_INT: return "int";
	case S_RANGE: return "range";
	default: return "";
	}
}

WithStats::Stat WithStats::stringToStat(const std::string &str) {
	if (str == "max_hp") return S_MAX_HP;
	else if (str == "max_mp") return S_MAX_MP;
	else if (str == "str") return S_STR;
	else if (str == "acc") return S_ACC;
	else if (str == "p_res") return S_P_RES;
	else if (str == "m_res") return S_M_RES;
	else if (str == "dxt") return S_DXT;
	else if (str == "ded") return S_DED;
	else if (str == "int") return S_INT;
	else if (str == "range") return S_RANGE;
	else return S_UNKNOWN;
}

WithStats::WithStats(const DescriptorWithStats *desc) : 
	stats(desc->stats), 
	missileTextureFileName(desc->missileTextureFileName) {
	ranged = desc->ranged;
	immobile = desc->immobile;
}

void WithStats::setStat(Stat S, int val) {
	for (unsigned i = 0; i < stats.size(); ++i) {
		if (stats[i].first == S) {
			stats[i].second = val;
			return;
		}
	}

	stats.push_back(StatVal(S, val));
}

int WithStats::getStat(Stat S) const {
	for (unsigned i = 0; i < stats.size(); ++i) {
		if (stats[i].first == S) {
			return stats[i].second;
		}
	}

	return 0;
}
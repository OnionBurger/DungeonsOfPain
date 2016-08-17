#ifndef _Storage_Effects_h_
#define _Storage_Effects_h_

#include <string>
#include <unordered_map>

class Effect;

class StorageEffects {
	std::unordered_map<std::string, Effect*> effects;

public:
	StorageEffects();

	void read();

	Effect* getEffect(const std::string &handle);

	void clear();

	~StorageEffects();
};

#endif
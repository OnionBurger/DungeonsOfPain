#ifndef _Descriptors_h_
#define _Descriptors_h_

#include <vector>
#include <unordered_map>

class DescriptorLevel;
class DescriptorAvatar;
class DescriptorEnemy;
class DescriptorItem;
class DescriptorBuff;
class StorageEffects;

class Descriptors {
	StorageEffects *effects;
	std::vector<DescriptorLevel*> levels;
	std::vector<DescriptorAvatar*> avatars;
	std::vector<DescriptorEnemy*> enemies;
	std::vector<DescriptorItem*> items;
	std::unordered_map<std::string, DescriptorBuff*> buffs;

	void readLevels();
	void readAvatars();
	void readEnemies();
	void readItems();
	void readBuffs();

public:
	Descriptors();

	void read();

	const StorageEffects* getEffects() const { return effects; }
	StorageEffects* getEffects() { return effects; }

	unsigned getDescLevelSize() const { return levels.size(); }
	const DescriptorLevel* getDescLevel(unsigned ind) const;
	DescriptorLevel* getDescLevel(unsigned ind);

	unsigned getDescAvatarSize() const { return avatars.size(); }
	const DescriptorAvatar* getDescAvatar(unsigned ind) const;

	unsigned getDescEnemySize() const { return enemies.size(); }
	const DescriptorEnemy* getDescEnemy(unsigned ind) const;
	DescriptorEnemy* getDescEnemy(unsigned ind);

	unsigned getDescItemSize() const { return items.size(); }
	const DescriptorItem* getDescItem(unsigned ind) const;
	DescriptorItem* getDescItem(unsigned ind);
	const DescriptorItem* getDescItem(const std::string &handle) const;
	DescriptorItem* getDescItem(const std::string &handle);

	const DescriptorBuff* getDescBuff(const std::string &handle) const;

	void clear();

	~Descriptors();
};

#endif
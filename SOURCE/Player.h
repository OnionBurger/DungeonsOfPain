#ifndef _Player_h_
#define _Player_h_

#include <Morph\Object.h>
#include "Combatant.h"
#include "WithTriggers.h"
#include <vector>

class DescriptorAvatar;
class Inventory;
class Tile;

class Player : public morph::Object, public Combatant {
	friend class Item;

	StorageEffects *effects;

	std::string handle, name;
	std::string textureFileName;
	
	std::vector<std::string> startingItems;

	Inventory *inv;
	
	bool drainAmmoSlot(const std::string &ammo);

public:
	static const morph::ObjType T_PLAYER;

public:
	Player(const DescriptorAvatar *desc, Inventory *I, StorageEffects *S);

	const std::string& getHandle() const { return handle; }

	morph::ObjType getType() const { return T_PLAYER; }

	int getStat(Stat S) const;
	int getRange() const;
	bool isRanged() const;
	std::string getMissileTextureFileName() const;

	bool isImmobile() const;

	bool hasManaFor(Item *item);
	bool spendManaFor(Item *item);

	bool drainAmmoForAttack(Tile *retrieveAmmoAt);
	
	void addStartingItems();
	bool pickupItem(Item *item);

	bool onTrigger(Trigger::Trigg tr);

	std::string combatStatsToString() const;

	void onDeath();

	void onQuitDungeon() { setPersistent(false); }
};

#endif
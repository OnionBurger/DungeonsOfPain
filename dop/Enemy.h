#ifndef _Enemy_h_
#define _Enemy_h_

#include <Morph\Object.h>
#include "Combatant.h"

class DescriptorEnemy;
class Map;

class Enemy : public morph::Object, public Combatant {
public:
	static const morph::ObjType T_ENEMY;

private:	
	std::string handle, name;
	std::string textureFileName;

	bool revealed;
	bool seen;

public:
	Enemy(const DescriptorEnemy *desc, StorageEffects *S);

	const std::string& getHandle() const { return handle; }

	morph::ObjType getType() const { return T_ENEMY; }

	void setRevealed(bool r);
	bool isRevealed() const { return revealed; }
	bool isSeen() const { return seen; }

	bool canAttackPlayer() const;

	void onDeath();

	std::string toString() const;
};

#endif
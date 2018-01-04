#include "Enemy.h"
#include <Morph\Looks\LookSprite.h>
#include "GlobalTextureCont.h"
#include "Player.h"
#include "DescriptorEnemy.h"
#include "DescriptorCombatant.h"
#include "Map.h"

const morph::ObjType Enemy::T_ENEMY = 10;

Enemy::Enemy(const DescriptorEnemy *desc, StorageEffects *S) : 
	morph::Object(10), Combatant(desc, S), 
	textureFileName(desc->textureFileName), 
	handle(desc->handle), 
	name(desc->name) {
	revealed = false;
	seen = false;

	setRendered(false);
	setLook(new morph::LookSprite(catchEnemyTexture(textureFileName), true));
}

void Enemy::setRevealed(bool r) {
	if (revealed == r) return;

	revealed = r;

	if (revealed) {
		if (!seen) {
			map->onEnemySee();
			seen = true;
		}
	}
	
	setRendered(revealed);
}

bool Enemy::canAttackPlayer() const {
	if (!revealed) return false;
	if (map->isPlayerDead()) return false;

	return inRangeForAttack(map->getPlayerX(), map->getPlayerY());
}

void Enemy::onDeath() {
	map->onEnemyDeath(this);
}

std::string Enemy::toString() const {
	return name + '\n' + combatStatsToString();
}
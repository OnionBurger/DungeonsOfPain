#ifndef _Info_Enemy_h_
#define _Info_Enemy_h_

#include "UIBox.h"

class Enemy;

class InfoEnemy : public UIBox {
	Enemy *enemyToShow;

public:
	InfoEnemy(const sf::Rect<float> &Border);

	void setEnemy(Enemy *enemy);
	Enemy* getEnemy() { return enemyToShow; }
	
	void refresh();

	void onQuitDungeon() { setPersistent(false); }
};

#endif
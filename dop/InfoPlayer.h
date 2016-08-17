#ifndef _Info_Player_h_
#define _Info_Player_h_

#include "UIBox.h"

class Player;

class InfoPlayer : public UIBox {
	Player *playerToShow;

public:
	InfoPlayer(const sf::Rect<float> &Border);

	void setPlayer(Player *player);
	
	void refresh();

	void onQuitDungeon() { setPersistent(false); }
};

#endif;
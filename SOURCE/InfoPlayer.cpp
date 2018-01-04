#include "InfoPlayer.h"
#include "Player.h"
#include <Morph\Looks\LookText.h>
#include "GlobalFontCont.h"
#include <string>

InfoPlayer::InfoPlayer(const sf::Rect<float> &Border) : UIBox(Border) {
	setPersistent(true);

	playerToShow = 0;

	morph::LookText *text = 
		new morph::LookText(globalFontCont.getSfmlRes("rsrcs/fonts/arial.ttf"));

	text->txt.setCharacterSize(16);
	text->move(sf::Vector2f(20, 20));

	setLook(text);
}

void InfoPlayer::setPlayer(Player *player) {
	playerToShow = player;
	refresh();
}

void InfoPlayer::refresh() {
	if (playerToShow) {
		((morph::LookText*)getLook())->txt.setString(playerToShow->combatStatsToString());
	} else {
		((morph::LookText*)getLook())->txt.setString("");
	}
}
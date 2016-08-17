#include "InfoEnemy.h"
#include "Enemy.h"
#include <Morph\Looks\LookText.h>
#include "GlobalFontCont.h"
#include <string>

InfoEnemy::InfoEnemy(const sf::Rect<float> &Border) : UIBox(Border) {
	setPersistent(true);

	enemyToShow = 0;

	morph::LookText *text = 
		new morph::LookText(globalFontCont.getSfmlRes("rsrcs/fonts/arial.ttf"));

	text->txt.setCharacterSize(16);
	text->move(sf::Vector2f(20, 20));

	setLook(text);
}

void InfoEnemy::setEnemy(Enemy *enemy) {
	enemyToShow = enemy;
	refresh();
}

void InfoEnemy::refresh() {
	if (enemyToShow) {
		((morph::LookText*)getLook())->txt.setString(enemyToShow->toString());
	} else {
		((morph::LookText*)getLook())->txt.setString("");
	}
}
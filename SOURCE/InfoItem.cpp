#include "InfoItem.h"
#include "Item.h"
#include <Morph\Looks\LookText.h>
#include "GlobalFontCont.h"
#include <string>

InfoItem::InfoItem(const sf::Rect<float> &Border) : UIBox(Border) {
	setPersistent(true);

	itemToShow = 0;

	morph::LookText *text = 
		new morph::LookText(globalFontCont.getSfmlRes("rsrcs/fonts/arial.ttf"));

	text->txt.setCharacterSize(16);
	text->move(sf::Vector2f(20, 20));

	setLook(text);
}

void InfoItem::setItem(Item *item) {
	itemToShow = item;
	refresh();
}

void InfoItem::refresh() {
	if (itemToShow) {
		((morph::LookText*)getLook())->txt.setString(itemToShow->toString());
	} else {
		((morph::LookText*)getLook())->txt.setString("");
	}
}
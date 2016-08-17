#include "UIBox.h"

UIBox::UIBox(const sf::Rect<float> &Border, int Depth) : morph::Object(Depth), border(Border) {
	move(border.left, border.top);
}
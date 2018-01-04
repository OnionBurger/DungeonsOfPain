#ifndef _UIBox_h_
#define _UIBox_h_

#include <Morph\Object.h>

class UIBox : public morph::Object {
protected:
	sf::Rect<float> border;

public:
	UIBox(const sf::Rect<float> &Border, int Depth = -20);

	virtual void refresh() =0;
	virtual void onCameraMove(const sf::Vector2f &offset) { move(offset); }

	virtual ~UIBox() {}
};

#endif
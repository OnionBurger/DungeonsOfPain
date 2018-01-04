#ifndef _Menu_Item_h_
#define _Menu_Item_h_

#include <Morph\Objects\ObjButton.h>
#include <string>

class MenuItem : public morph::ObjButton {
public:
	MenuItem(sf::Vector2f startCoords, const std::string &textureName);

	virtual void onClick() =0;

	virtual ~MenuItem() {}
};

#endif
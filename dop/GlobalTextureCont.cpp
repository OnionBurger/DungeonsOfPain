#include "GlobalTextureCont.h"

morph::TextureContainer globalTextureCont;

const sf::Texture& catchAvatarTexture(const std::string &fileName) {
	try {
		return globalTextureCont.getSfmlRes("rsrcs/pics/avatars/" + fileName);
	} catch (morph::TextureContainer::ErrNotFound e) {
		return globalTextureCont.getSfmlRes("rsrcs/pics/avatars/unknown_avatar.png");
	}
}

const sf::Texture& catchEnemyTexture(const std::string &fileName) {
	try {
		return globalTextureCont.getSfmlRes("rsrcs/pics/enemies/" + fileName);
	} catch (morph::TextureContainer::ErrNotFound e) {
		return globalTextureCont.getSfmlRes("rsrcs/pics/enemies/unknown_enemy.png");
	}
}

const sf::Texture& catchItemTexture(const std::string &fileName) {
	try {
		return globalTextureCont.getSfmlRes("rsrcs/pics/items/" + fileName);
	} catch (morph::TextureContainer::ErrNotFound e) {
		return globalTextureCont.getSfmlRes("rsrcs/pics/items/unknown_item.png");
	}
}

const sf::Texture& catchWallTexture(const std::string &fileName) {
	try {
		return globalTextureCont.getSfmlRes("rsrcs/pics/walls/" + fileName);
	} catch (morph::TextureContainer::ErrNotFound e) {
		return globalTextureCont.getSfmlRes("rsrcs/pics/walls/wall.png");
	}
}

const sf::Texture& catchDoorTexture(const std::string &fileName) {
	try {
		return globalTextureCont.getSfmlRes("rsrcs/pics/doors/" + fileName);
	} catch (morph::TextureContainer::ErrNotFound e) {
		return globalTextureCont.getSfmlRes("rsrcs/pics/doors/door.png");
	}
}

const sf::Texture& catchFloorTexture(const std::string &fileName) {
	try {
		return globalTextureCont.getSfmlRes("rsrcs/pics/floors/" + fileName);
	} catch (morph::TextureContainer::ErrNotFound e) {
		return globalTextureCont.getSfmlRes("rsrcs/pics/floors/floor.png");
	}
}

const sf::Texture& catchStairsTexture(const std::string &fileName) {
	try {
		return globalTextureCont.getSfmlRes("rsrcs/pics/stairs/" + fileName);
	} catch (morph::TextureContainer::ErrNotFound e) {
		return globalTextureCont.getSfmlRes("rsrcs/pics/stairs/stairs.png");
	}
}

const sf::Texture& catchFxTexture(const std::string &fileName) {
	try {
		return globalTextureCont.getSfmlRes("rsrcs/pics/fx/" + fileName);
	} catch (morph::TextureContainer::ErrNotFound e) {
		return globalTextureCont.getSfmlRes("rsrcs/pics/fx/unknown_fx.png");
	}
}

const sf::Texture& catchMenuTexture(const std::string &fileName) {
	return globalTextureCont.getSfmlRes("rsrcs/pics/menu/" + fileName);
}

const sf::Texture& catchUITexture(const std::string &fileName) {
	return globalTextureCont.getSfmlRes("rsrcs/pics/ui/" + fileName);
}
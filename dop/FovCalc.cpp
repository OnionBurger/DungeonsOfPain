#include "FovCalc.h"
#include "Map.h"
#include "Tile.h"

FovCalc::FovCalc(Map *M) {
	map = M;

	fov_settings_init(&fov_settings);
    fov_settings_set_opacity_test_function(&fov_settings, libfovOpacityTest);
    fov_settings_set_apply_lighting_function(&fov_settings, libfovApplyLighting);
}

void FovCalc::calcFov() {
	if (map == 0 || map->player == 0) return;

	map->unrevealTiles();

	map->tiles[map->getPlayerX()][map->getPlayerY()]->setRevealed(true);
	fov_circle(&fov_settings, map, 0, map->getPlayerX(), map->getPlayerY(), 10);
}

bool FovCalc::libfovOpacityTest(void *map, int x, int y) {
	Map *M = (Map*)map;

	if (!M->validIntCoords(x, y)) return true;

	return M->tiles[x][y]->blocksLight();
}

void FovCalc::libfovApplyLighting(void *map, int x, int y, int dx, int dy, void *src) {
	Map *M = (Map*)map;
	
	if (!M->validIntCoords(x, y)) return;

	M->tiles[x][y]->setRevealed(true);
}
#ifndef _FovCalc_h_
#define _FovCalc_h_

#include "fov.h"

class Map;

class FovCalc {
	fov_settings_type fov_settings;

	Map *map;

public:
	FovCalc(Map *M);

	void calcFov();

	static bool libfovOpacityTest(void *map, int x, int y);
	static void libfovApplyLighting(void *map, int x, int y, int dx, int dy, void *src);
};

#endif
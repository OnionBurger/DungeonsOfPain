#include "getRand.h"
#include <cstdlib>

float getRandFloat(float upperExclusive) {
	return (((float)rand())/RAND_MAX) * upperExclusive;
}

unsigned getRandUnsigned(unsigned upperExclusive) {
	return (unsigned)(getRandFloat(upperExclusive));
}
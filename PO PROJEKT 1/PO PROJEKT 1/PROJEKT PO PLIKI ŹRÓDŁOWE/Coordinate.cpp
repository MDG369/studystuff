#include "Coordinate.h"

bool Coordinate::operator<(const Coordinate& o) const {
	return x < o.x || (x == o.x && y < o.y);
}

bool Coordinate::operator==(const Coordinate& o) const {
	if (x == o.x && y == o.y) {
		return true;
	}
	else {
		return false;
	}
}

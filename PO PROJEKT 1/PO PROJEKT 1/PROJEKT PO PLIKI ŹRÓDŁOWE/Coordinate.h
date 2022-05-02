#pragma once
struct Coordinate {
	int x;
	int y;
	bool operator<(const Coordinate& o) const;
	bool operator==(const Coordinate& o) const;
};

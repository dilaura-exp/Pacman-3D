#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>

struct Coordinate {
	float x, y, z;
	
	Coordinate(float x, float y, float z) {
		this->x = x; this->y = y; this->z = z;
	}
};

struct Face {
	bool quad;
	int vertexIndexes[4];
	int normalIndexes[4];

	Face(int v[4], int vn[4], bool quad) {
		vertexIndexes[0] = v[0];
		vertexIndexes[1] = v[1];
		vertexIndexes[2] = v[2];
		vertexIndexes[3] = v[3];
		normalIndexes[0] = vn[0];
		normalIndexes[1] = vn[1];
		normalIndexes[2] = vn[2];
		normalIndexes[3] = vn[3];
		this->quad = quad;
	}
};

class ObjectLoader {
public:
	static int loadWavefront(const char* filePath);
};


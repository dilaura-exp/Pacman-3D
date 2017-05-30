#pragma once
#include "GameObject.h"
#include "Node.h"
#include <vector>
#include <fstream>
#include "ErrorLog.h"

using namespace std;

class Grid {
public:
	Grid();
	~Grid();
	
	void init(float nodeRadius);
	void draw(int currentY);
	void loadLevel(const char* filePath);
	Node ****getNodes();
	void setNodes(int i, int j, int k, Node *node);
	int getSizeX();
	int getSizeY();
	int getSizeZ();
	int getMaxSize();
	vector<Node*> getNeighbours(Node *node);
	Node *nodeFromWorldPoint(Vector3 worldPosition);

private:
	void drawBottomPlane(float radius, float material[]);
	void drawCube(float radius, float material[]);
	void drawHalfCube(float radius, float material[]);
	void drawLadder(float radius, float material[]);

	Vector3 gridWorldSize;
	float nodeRadius;
	Node ****nodes;
	float nodeDiameter;
	int gridSizeX, gridSizeY, gridSizeZ;
};


#include "Grid.h"
#include <iostream>

using namespace std;

Grid::Grid() {
}

Grid::~Grid() {
}

void Grid::init(Vector3 gridWorldSize, float nodeRadius) {
	this->gridWorldSize = gridWorldSize;
	this->nodeRadius = nodeRadius;
	nodeDiameter = nodeRadius * 2;
	gridSizeX = round(gridWorldSize.x / nodeDiameter);
	gridSizeY = round(gridWorldSize.y / nodeDiameter);
	gridSizeZ = round(gridWorldSize.z / nodeDiameter);
	createGrid();
}

void Grid::draw() {
	for (int i = gridSizeX-1; i >= 0 ; i--) {
		for (int j = gridSizeY-1; j >= 0 ; j--) {
			for (int k = gridSizeZ-1; k >= 0; k--) {
				glPushMatrix();
				glTranslatef(nodes[i][j][k]->worldPosition->x, nodes[i][j][k]->worldPosition->y, nodes[i][j][k]->worldPosition->z);
					float ambdiv[]{ 0.25f, 0.25f, 0.75f, 0.5 };
					drawBottomPlane(nodeRadius, ambdiv);
				glPopMatrix();
			}
		}
	}
}

void Grid::drawBottomPlane(float radius, float material[]) {
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
	glBegin(GL_QUADS);
		// bottom
		glVertex3f(radius, -radius, radius);
		glVertex3f(-radius, -radius, radius);
		glVertex3f(-radius, -radius, -radius);
		glVertex3f(radius, -radius, -radius);
	glEnd();
}

void Grid::drawCube(float radius, float material[]) {
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);
	glBegin(GL_QUADS);
		// top
		glVertex3f(-radius, radius, radius);
		glVertex3f(radius, radius, radius);
		glVertex3f(radius, radius, -radius);
		glVertex3f(-radius, radius, -radius);

		// front
		glVertex3f(radius, -radius, radius);
		glVertex3f(radius, radius, radius);
		glVertex3f(-radius, radius, radius);
		glVertex3f(-radius, -radius, radius);

		// right
		glVertex3f(radius, radius, -radius);
		glVertex3f(radius, radius, radius);
		glVertex3f(radius, -radius, radius);
		glVertex3f(radius, -radius, -radius);

		// left
		glVertex3f(-radius, -radius, radius);
		glVertex3f(-radius, radius, radius);
		glVertex3f(-radius, radius, -radius);
		glVertex3f(-radius, -radius, -radius);

		// bottom
		glVertex3f(radius, -radius, radius);
		glVertex3f(-radius, -radius, radius);
		glVertex3f(-radius, -radius, -radius);
		glVertex3f(radius, -radius, -radius);

		// back
		glVertex3f(radius, radius, -radius);
		glVertex3f(radius, -radius, -radius);
		glVertex3f(-radius, -radius, -radius);
		glVertex3f(-radius, radius, -radius);
	glEnd();
}

void Grid::loadLevel(string level) {

}

Node ****Grid::getNodes() {
	return nodes;
}

void Grid::setNodes(int i, int j, int k, Node *node) {
	nodes[i][j][k] = node;
}

int Grid::getSizeX() {
	return gridSizeX;
}

int Grid::getSizeY() {
	return gridSizeY;
}

int Grid::getSizeZ() {
	return gridSizeZ;
}

int Grid::getMaxSize() {
	return gridSizeX * gridSizeY * gridSizeZ;
}

void Grid::createGrid() {
	nodes = new Node***[gridSizeX];
	for (int i = 0; i < gridSizeX; i++) {
		nodes[i] = new Node**[gridSizeY];
		for (int j = 0; j < gridSizeY; j++) {
			nodes[i][j] = new Node*[gridSizeZ];
		}
	}

	Vector3 worldBottomLeft = (Vector3(1, 0, 0) * gridWorldSize.x / 2) - (Vector3(0, 1, 0) * gridWorldSize.y / 2) - (Vector3(0, 0, 1) * gridWorldSize.z / 2);

	for (int x = 0; x<gridSizeX; x++) {
		for (int y = 0; y < gridSizeY; y++) {
			for (int z = 0; z < gridSizeZ; z++) {
				Vector3 *worldPoint = new Vector3(worldBottomLeft + Vector3(1, 0, 0) * (x * nodeDiameter + nodeRadius) + Vector3(0, 1, 0) * (y * nodeDiameter + nodeRadius) + Vector3(0, 0, 1) * (z * nodeDiameter + nodeRadius));
				bool walkable = true;
				nodes[x][y][z] = new Node(walkable, worldPoint, x, y, z);
			}
		}
	}
}

vector<Node*> Grid::getNeighbours(Node *node) {
	vector<Node*> neighbours;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			for (int z = -1; z <= 1; z++) {
				if (x == 0 && y == 0 && z == 0) {
					continue;
				}
				if ((x == 0 && y == 0) || (x == 0 && z == 0) ||	(y == 0 && z == 0)) {
					int checkX = node->gridX + x;
					int checkY = node->gridY + y;
					int checkZ = node->gridZ + z;

					if (checkX >= 0 && checkX < gridSizeX &&
						checkY >= 0 && checkY < gridSizeY &&
						checkZ >= 0 && checkZ < gridSizeZ) {
						neighbours.push_back(nodes[checkX][checkY][checkZ]);
					}
				}
			}
		}
	}
	return neighbours;
}

float clamp01(float value) {
	if (value > 1) return 1;
	else if (value < 0) return 0;
	else return value;
}

Node *Grid::nodeFromWorldPoint(Vector3 worldPosition) {
	float percentX = (worldPosition.x + gridWorldSize.x / 2) / gridWorldSize.x;
	float percentY = (worldPosition.y + gridWorldSize.y / 2) / gridWorldSize.y;
	float percentZ = (worldPosition.z + gridWorldSize.z / 2) / gridWorldSize.z;

	percentX = clamp01(percentX);
	percentY = clamp01(percentY);
	percentZ = clamp01(percentZ);

	int x = (int) round((gridSizeX - 1) * percentX);
	int y = (int) round((gridSizeY - 1) * percentY);
	int z = (int) round((gridSizeZ - 1) * percentZ);
	return nodes[x][y][z];
}

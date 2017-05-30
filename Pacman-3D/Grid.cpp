#include "Grid.h"
#include <iostream>

using namespace std;

Grid::Grid() {
}

Grid::~Grid() {
}

void Grid::init(float nodeRadius) {
	this->nodeRadius = nodeRadius;
	nodeDiameter = nodeRadius * 2;
	loadLevel("Level.txt");
}

/*
for (int i = gridSizeX-1; i >= 0 ; i--) {
for (int j = gridSizeY-1; j >= 0 ; j--) {
for (int k = gridSizeZ-1; k >= 0; k--) {
*/

/*
for (int i = 0; i < gridSizeX; i++) {
for (int j = 0; j < gridSizeY; j++) {
for (int k = 0; k < gridSizeZ; k++) {
*/

void Grid::draw(int currentY) {
	glLineWidth(1.5);
	for (int i = gridSizeX - 1; i >= 0; i--) {
		for (int j = gridSizeY - 1; j >= 0; j--) {
			for (int k = gridSizeZ - 1; k >= 0; k--) {
				float alpha = (j > currentY) ? 0.15 : 0.75f;
				glPushMatrix();
					glTranslatef(nodes[i][j][k]->worldPosition->x, nodes[i][j][k]->worldPosition->y, nodes[i][j][k]->worldPosition->z);
					if (nodes[i][j][k]->walkable) {
						if (nodes[i][j][k]->stairType == 1) {
							float ambdiv[]{ 0.5f, 0.75f, 0.75f, alpha };
							drawLadder(nodeRadius, ambdiv);
						}
						else if (nodes[i][j][k]->stairType == 2) {
							float ambdiv[]{ 0.5f, 0.75f, 0.75f, alpha };
							drawBottomPlane(nodeRadius, ambdiv);
						}
						else {
							float ambdiv[]{ 0.25f, 0.25f, 0.75f, alpha };
							drawBottomPlane(nodeRadius, ambdiv);
						}
					}
					else {
						if (nodes[i][j][k]->obstacle) {
							float ambdiv[]{ 0.75f, 0.25f, 0.25f, alpha };
							drawHalfCube(nodeRadius, ambdiv);
						}
					}
				glPopMatrix();
			}
		}
	}
}

void Grid::drawBottomPlane(float radius, float material[]) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float black[]{ 0, 0, 0, material[3]};
	glColor4f(black[0], black[1], black[2], black[3]);
	for (int i = 0; i < 2; i++) {
			glBegin(GL_QUADS);
			// bottom
			glVertex3f(radius, -radius, radius);
			glVertex3f(-radius, -radius, radius);
			glVertex3f(-radius, -radius, -radius);
			glVertex3f(radius, -radius, -radius);
		glEnd();
		radius -= 0.01f;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4f(material[0], material[1], material[2], material[3]);
	}
}

void Grid::drawCube(float radius, float material[]) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float black[]{ 0, 0, 0, material[3] };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);
	for (int i = 0; i < 2; i++) {
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
		radius -= 0.01f;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);
	}
}

void Grid::drawLadder(float radius, float material[]) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float black[]{ 0, 0, 0, material[3] };
	glColor4f(black[0], black[1], black[2], black[3]);
	for (int i = 0; i < 2; i++) {
		glBegin(GL_QUADS);

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

		// back
		glVertex3f(radius, radius, -radius);
		glVertex3f(radius, -radius, -radius);
		glVertex3f(-radius, -radius, -radius);
		glVertex3f(-radius, radius, -radius);
		glEnd();
		radius -= 0.01f;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4f(material[0], material[1], material[2], material[3]);
	}
}

void Grid::drawHalfCube(float radius, float material[]) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	float black[]{ 0, 0, 0, material[3] };
	glColor4f(black[0], black[1], black[2], black[3]);
	for (int i = 0; i < 2; i++) {
		glBegin(GL_QUADS);
		// top
		glVertex3f(-radius, 0, radius);
		glVertex3f(radius, 0, radius);
		glVertex3f(radius, 0, -radius);
		glVertex3f(-radius, 0, -radius);

		// front
		glVertex3f(radius, -radius, radius);
		glVertex3f(radius, 0, radius);
		glVertex3f(-radius, 0, radius);
		glVertex3f(-radius, -radius, radius);

		// right
		glVertex3f(radius, 0, -radius);
		glVertex3f(radius, 0, radius);
		glVertex3f(radius, -radius, radius);
		glVertex3f(radius, -radius, -radius);

		// left
		glVertex3f(-radius, -radius, radius);
		glVertex3f(-radius, 0, radius);
		glVertex3f(-radius, 0, -radius);
		glVertex3f(-radius, -radius, -radius);

		// bottom
		glVertex3f(radius, -radius, radius);
		glVertex3f(-radius, -radius, radius);
		glVertex3f(-radius, -radius, -radius);
		glVertex3f(radius, -radius, -radius);

		// back
		glVertex3f(radius, 0, -radius);
		glVertex3f(radius, -radius, -radius);
		glVertex3f(-radius, -radius, -radius);
		glVertex3f(-radius, 0, -radius);
		glEnd();
		radius -= 0.01f;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4f(material[0], material[1], material[2], material[3]);
	}
}

void Grid::loadLevel(const char* filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		logFatalError("Can not load level. File can not be opened.");
	}
	std::string temp;
	Vector3 worldBottomLeft;
	int x, y, z;
	x = y = z = 0;
	while (std::getline(file, temp)) {
		if (temp[0] == 's' && temp[1] == ' ') {
			sscanf_s(temp.c_str(), "s %d x %d x %d", &gridSizeX, &gridSizeY, &gridSizeZ);
			nodes = new Node***[gridSizeX];
			for (int i = 0; i < gridSizeX; i++) {
				nodes[i] = new Node**[gridSizeY];
				for (int j = 0; j < gridSizeY; j++) {
					nodes[i][j] = new Node*[gridSizeZ];
				}
			}
			gridWorldSize.x = gridSizeX * nodeDiameter;
			gridWorldSize.y = gridSizeY * nodeDiameter;
			gridWorldSize.z = gridSizeZ * nodeDiameter;
			worldBottomLeft = (Vector3(1, 0, 0) * gridWorldSize.x / 2) - (Vector3(0, 1, 0) * gridWorldSize.y / 2) - (Vector3(0, 0, 1) * gridWorldSize.z / 2);
		}
		else if (temp[0] == '#') {
			cout << endl;
			x = z = 0;
			y++;
			continue;
		}
		else if (temp[0] == 'x' || temp[0] == '.' || temp[0] == 'H' || temp[0] == '-' || temp[0] == 'h') {
			cout << temp << endl;
			for (x = 0; x < temp.size(); x++) {
				Vector3 *worldPoint = new Vector3(worldBottomLeft + Vector3(1, 0, 0) * (x * nodeDiameter + nodeRadius) + Vector3(0, 1, 0) * (y * nodeDiameter + nodeRadius) + Vector3(0, 0, 1) * (z * nodeDiameter + nodeRadius));
				bool walkable = true;
				int stairType = 0;
				bool obstacle = false;
				if (temp[x] == 'x') {
					walkable = false;
					obstacle = true;
				}
				else if (temp[x] == '-') {
					walkable = false;
				}
				else if (temp[x] == 'H') {
					stairType = 1;
				} 
				else if (temp[x] == 'h') {
					stairType = 2;
				}
				nodes[x][y][z] = new Node(walkable, worldPoint, x, y, z);
				nodes[x][y][z]->stairType = stairType;
				nodes[x][y][z]->obstacle = obstacle;
			}
			z++;
		}
	}
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

#include "ObjectLoader.h"
#include "ErrorLog.h"

#include <iostream>

int ObjectLoader::loadWavefront(const char* filePath) {
	std::vector<std::string*> coord;
	std::vector<Coordinate*> vertices;
	std::vector<Face*> faces;
	std::vector<Coordinate*> normals;
	std::ifstream in(filePath);
	if (!in.is_open()) {
		logFatalError("Can not load wavefront. File can not be opened.");
	}
	char buf[256];
	while (!in.eof()) {		//eof end of the file
		in.getline(buf, 256);
		coord.push_back(new std::string(buf));
	}
	for (int i = 0; i < coord.size(); i++) {
		if ((*coord[i])[0] == '#')
			continue;
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == ' ') {
			char tmp;
			float tmpX, tmpY, tmpZ;
			sscanf_s(coord[i]->c_str(), "v %f %f %f", &tmpX, &tmpY, &tmpZ);
			vertices.push_back(new Coordinate(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 'n') {
			float tmpX, tmpY, tmpZ;
			sscanf_s(coord[i]->c_str(), "vn %f %f %f", &tmpX, &tmpY, &tmpZ);
			normals.push_back(new Coordinate(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'f') {
			int pos[4];
			int texcoord[4];
			int normal[4];
			if (coord[i]->find("//") != std::string::npos) {
				if (count(coord[i]->begin(), coord[i]->end(), ' ') == 4) {
					sscanf_s(coord[i]->c_str(), "f %d//%d %d//%d %d//%d %d//%d", &pos[0], &normal[0], &pos[1], &normal[1], &pos[2], &normal[2], &pos[3], &normal[3]);
					faces.push_back(new Face(pos, normal, true));
				}
				else {
					sscanf_s(coord[i]->c_str(), "f %d//%d %d//%d %d//%d", &pos[0], &normal[0], &pos[1], &normal[1], &pos[2], &normal[2]);
					faces.push_back(new Face(pos, normal, false));
				}
			}
			else {
				if (count(coord[i]->begin(), coord[i]->end(), ' ') == 4) {
					sscanf_s(coord[i]->c_str(), "f %d/&d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &pos[0], &texcoord[0], &normal[0], &pos[1], &texcoord[1], &normal[1], &pos[2], &texcoord[2], &normal[2], &pos[3], &texcoord[3], &normal[3]);
					faces.push_back(new Face(pos, normal, true));
				}
				else {
					sscanf_s(coord[i]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &pos[0], &texcoord[0], &normal[0], &pos[1], &texcoord[1], &normal[1], &pos[2], &texcoord[2], &normal[2]);
					faces.push_back(new Face(pos, normal, false));
				}
			}
		}
	}

	int num;
	num = glGenLists(1);
	glNewList(num, GL_COMPILE);
	float longest = 0;
		for (int i = 0; i < faces.size(); i++) {
			if (faces[i]->quad) {
				glBegin(GL_QUADS);
					glNormal3f(normals[faces[i]->normalIndexes[0] - 1]->x, normals[faces[i]->normalIndexes[0] - 1]->y, normals[faces[i]->normalIndexes[0] - 1]->z);
					glVertex3f(vertices[faces[i]->vertexIndexes[0] - 1]->x, vertices[faces[i]->vertexIndexes[0] - 1]->y, vertices[faces[i]->vertexIndexes[0] - 1]->z);
					glNormal3f(normals[faces[i]->normalIndexes[1] - 1]->x, normals[faces[i]->normalIndexes[1] - 1]->y, normals[faces[i]->normalIndexes[1] - 1]->z);
					glVertex3f(vertices[faces[i]->vertexIndexes[1] - 1]->x, vertices[faces[i]->vertexIndexes[1] - 1]->y, vertices[faces[i]->vertexIndexes[1] - 1]->z);
					glNormal3f(normals[faces[i]->normalIndexes[2] - 1]->x, normals[faces[i]->normalIndexes[2] - 1]->y, normals[faces[i]->normalIndexes[2] - 1]->z);
					glVertex3f(vertices[faces[i]->vertexIndexes[2] - 1]->x, vertices[faces[i]->vertexIndexes[2] - 1]->y, vertices[faces[i]->vertexIndexes[2] - 1]->z);
					glNormal3f(normals[faces[i]->normalIndexes[3] - 1]->x, normals[faces[i]->normalIndexes[3] - 1]->y, normals[faces[i]->normalIndexes[3] - 1]->z);
					glVertex3f(vertices[faces[i]->vertexIndexes[3] - 1]->x, vertices[faces[i]->vertexIndexes[3] - 1]->y, vertices[faces[i]->vertexIndexes[3] - 1]->z);
				glEnd();
			}
			else {
				glBegin(GL_TRIANGLES);
					glNormal3f(normals[faces[i]->normalIndexes[0] - 1]->x, normals[faces[i]->normalIndexes[0] - 1]->y, normals[faces[i]->normalIndexes[0] - 1]->z);
					glVertex3f(vertices[faces[i]->vertexIndexes[0] - 1]->x, vertices[faces[i]->vertexIndexes[0] - 1]->y, vertices[faces[i]->vertexIndexes[0] - 1]->z);
					glNormal3f(normals[faces[i]->normalIndexes[1] - 1]->x, normals[faces[i]->normalIndexes[1] - 1]->y, normals[faces[i]->normalIndexes[1] - 1]->z);
					glVertex3f(vertices[faces[i]->vertexIndexes[1] - 1]->x, vertices[faces[i]->vertexIndexes[1] - 1]->y, vertices[faces[i]->vertexIndexes[1] - 1]->z);
					glNormal3f(normals[faces[i]->normalIndexes[2] - 1]->x, normals[faces[i]->normalIndexes[2] - 1]->y, normals[faces[i]->normalIndexes[2] - 1]->z);
					glVertex3f(vertices[faces[i]->vertexIndexes[2] - 1]->x, vertices[faces[i]->vertexIndexes[2] - 1]->y, vertices[faces[i]->vertexIndexes[2] - 1]->z);
				glEnd();
			}
			for (int j = 0; j < 4; j++) {
				float banding = vertices[faces[i]->vertexIndexes[j] - 1]->x;
				if (banding > longest) {
					longest = banding;
				}
			}
		}
		std::cout << longest << std::endl;
	glEndList();

	for (int i = 0; i < coord.size(); i++)
		delete coord[i];
	for (int i = 0; i < faces.size(); i++)
		delete faces[i];
	for (int i = 0; i < normals.size(); i++)
		delete normals[i];
	for (int i = 0; i < vertices.size(); i++)
		delete vertices[i];

	return num;
}

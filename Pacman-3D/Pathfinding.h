/*
Open    //Kumpulan node yang akan dilakukan pengecekan
Closed  //Kumpulan node yang sudah dicek

masukkan node awal ke dalam Open
loop
	current = node dalam Open dengan fcost paling rendah    //fcost = gcost (jarak dari awal) + hcost (jarak sampai tujuan)
	hapus current dari Open
	masukkan current ke Closed
	jika current == target node     //path sudah ditemukan
		return
	untuk setiap neighbour dari current node
		jika neighbour tidak dapat dilalui || neighbour di dalam Closed
			skip ke neighbour selanjutnya
		jika path baru dari neighbour lebih dekat || neighbour tidak di dalam Open
			set fcost neighbour
			set current sebagai parent dari neighbour
			jika neighbour tidak di dalam Open
				masukkan neighbour ke Open
*/

#pragma once
#include "Grid.h"
#include "NodeHeap.h"
#include <unordered_set>
#include <list>

class Pathfinding {
public:
	static Pathfinding &getInstance();
	~Pathfinding();

	void init(Grid *grid);
	static vector<Node*> findPath(Node *startNode, Node *targetNode);

private:
	static vector<Node*> makePath(Node *startNode, Node *targetNode);
	static int getDistance(Node *nodeA, Node *nodeB);

	Grid *grid;
};


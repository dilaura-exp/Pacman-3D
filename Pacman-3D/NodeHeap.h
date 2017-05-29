#pragma once
#include "Node.h"

class NodeHeap {
public:
	NodeHeap(int maxSize);
	~NodeHeap();

	void add(Node *node);
	Node *removeFirstNode();
	void updateItem(Node *node);
	int count();
	bool contains(Node *node);


private:
	void sortUp(Node *node);
	void sortDown(Node *node);
	void swap(Node *node1, Node *node2);

	Node **nodes;
	int currentItemCount;
};


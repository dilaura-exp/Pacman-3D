#include "NodeHeap.h"



NodeHeap::NodeHeap(int maxSize) {
	nodes = new Node*[maxSize];
}

NodeHeap::~NodeHeap() {
}

void NodeHeap::add(Node *node) {
	node->setHeapIndex(currentItemCount);
	nodes[currentItemCount] = node;
	sortUp(node);
	currentItemCount++;
}

void NodeHeap::sortUp(Node *node) {
	int parentIndex = (node->getHeapIndex() - 1) / 2;
	while (true) {
		Node *parentNode = nodes[parentIndex];
		if (node->CompareTo(parentNode) > 0) {
			swap(node, parentNode);
		}
		else {
			break;
		}
		parentIndex = (node->getHeapIndex() - 1) / 2;
	}
}

void NodeHeap::sortDown(Node *node) {
	while (true) {
		int childIndexLeft = node->getHeapIndex() * 2 + 1;
		int childIndexRight = node->getHeapIndex() * 2 + 2;
		int swapIndex = 0;

		if (childIndexLeft < currentItemCount) {
			swapIndex = childIndexLeft;
			if (childIndexRight < currentItemCount) {
				if (nodes[childIndexLeft]->CompareTo(nodes[childIndexRight]) < 0) {
					swapIndex = childIndexRight;
				}
			}
			if (node->CompareTo(nodes[swapIndex]) < 0) {
				swap(node, nodes[swapIndex]);
			}
			else {
				return;
			}
		}
		else {
			return;
		}
	}
}

void NodeHeap::swap(Node *node1, Node *node2) {
	nodes[node1->getHeapIndex()] = node2;
	nodes[node2->getHeapIndex()] = node1;
	int tempIndex = node1->getHeapIndex();
	node1->setHeapIndex(node2->getHeapIndex());
	node2->setHeapIndex(tempIndex);
}

Node *NodeHeap::removeFirstNode() {
	Node *firstNode = nodes[0];
	currentItemCount--;
	nodes[0] = nodes[currentItemCount];
	nodes[0]->setHeapIndex(0);
	sortDown(nodes[0]);
	return firstNode;
}

void NodeHeap::updateItem(Node *node) {
	sortUp(node);
}

int NodeHeap::count() {
	return currentItemCount;
}

bool NodeHeap::contains(Node *node) {
	return nodes[node->getHeapIndex()] == node;
}
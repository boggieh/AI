#pragma once

#include <iostream>
#include <vector>
struct Node
{
	int index;
	bool bVisited = false;			// Have we searched this node before?
	float fGlobalGoal;				// Distance to goal so far
	float fLocalGoal;				// Distance to goal if we took the alternative route
	int x;							// Nodes position in 2D space
	int y;
	std::vector<Node*> vecNeighbours;	// Connections to neighbours
	Node* parent;					// Node connecting to this node that offers shortest parent
};



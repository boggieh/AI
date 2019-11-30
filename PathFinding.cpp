#include <iostream>
#include <list>
#include "FileReader.h"
#include "FileWriter.h"
#include "Node.h"

#pragma warning(disable : 4996)

Node* CreateNodes(std::vector<int> values)
{
	int n = values.front();
	Node* nodes = new Node[n];

	for (int i = 1; i <= n * 2; i++)
	{
		int index = (i - 1) / 2;
		Node node;
		node.index = index + 1;
		node.x = values[i];
		node.y = values[++i];
		nodes[index] = node;
	}

	values.erase(values.begin(), values.begin() + 2 * n + 1);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int index = i * n + j;

			if (values[index] == 1)
			{
				nodes[j].vecNeighbours.push_back(&nodes[i]);
			}
		}
	}

	return nodes;
}

void Solve_AStar(Node* nodes, int nodeCount)
{

	
	for (int i = 0; i < nodeCount; i++) 
	{
		nodes[i].bVisited = false;
		nodes[i].fGlobalGoal = INFINITY;
		nodes[i].fLocalGoal = INFINITY;
		nodes[i].parent = nullptr;
	}

	Node* nodeStart = &nodes[0]; //start from first node
	Node* nodeEnd = &nodes[nodeCount - 1]; 

	auto heuristic = [](Node* a, Node* b)
	{
		float deltaX = b->x - a->x;
		float deltaY = b->y - a->y;
		return sqrtf(deltaX * deltaX + deltaY * deltaY); 
	};

	Node* nodeCurrent = nodeStart; //make current node first node with local goal 0 and global goal the heuristic calculation (sqrt of x^2 + y^2)
	nodeStart->fLocalGoal = 0.0f;
	nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

	std::list<Node*> listNotTestedNodes; //create a list for untested nodes and push it to 1st node
	listNotTestedNodes.push_back(nodeStart);

	while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)
	{
		//while the list is not empty and current node is not last node, sort nodes on left and right hand side
		listNotTestedNodes.sort([](const Node* lhs, const Node* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited) //put front (first node) in visited nodes and start again
			listNotTestedNodes.pop_front();

		if (listNotTestedNodes.empty())
			break;

		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->bVisited = true;

		for (Node * nodeNeighbour : nodeCurrent->vecNeighbours)
		{
			if (!nodeNeighbour->bVisited) //if the neighbour node isnt visited, go back to it
				listNotTestedNodes.push_back(nodeNeighbour);

			float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + heuristic(nodeCurrent, nodeNeighbour);

			//if there is a neighbour node with heuristic less than the current lowest goal, update parents, update new lowest goal
			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal) 
			{
				nodeNeighbour->parent = nodeCurrent;
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

				nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
			}
			
		}
		
	}
}

std::vector<int> GetSequence(Node* nodes, int nodeCount)
{
	Node* current = &nodes[nodeCount - 1];
	std::vector<int> vNodes;

	while (current != &nodes[0])
	{
		int index = current->index;
		vNodes.push_back(index);
		current = current->parent;
	}

	int index = current->index;
	vNodes.push_back(index);

	return vNodes;
}


char* GetFileName(char* fileName, const char* ext)
{
	char* outputFile = (char*)malloc(strlen(fileName) + strlen(ext) + 1);
	strcpy(outputFile, fileName);
	strcat(outputFile, ext);
	return outputFile;
}

void WriteOutputContent(Node *nodes, int nodeCount, char* outputFile)
{
	FileWriter writer(outputFile);
	if (!nodes[nodeCount - 1].bVisited)
		writer.WriteToFile(0);
	else
	{
		std::vector<int> nodeIndexes = GetSequence(nodes, nodeCount);

		for (int i = nodeIndexes.size() - 1; i >= 0; i--)
		{
			int index = nodeIndexes[i];
			writer.WriteToFile(index);
			if (i != 0)
				writer.WriteToFile(' ');
		}
	}
}

int main(int argc, char *argv[])
{
	char* fileName = argv[1];
	char* inputFile = GetFileName(fileName, ".cav"); //open test file as .cav
	FileReader reader(inputFile);
	std::vector<int> values = reader.GetContents();

	int nodeCount = values.front();
	Node* nodes = CreateNodes(values);

	Solve_AStar(nodes, nodeCount);
	
	char* outputFile = GetFileName(fileName, ".csn"); //save output as .csn
	WriteOutputContent(nodes, nodeCount, outputFile);

}
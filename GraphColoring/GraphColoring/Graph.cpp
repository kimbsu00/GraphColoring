#include "Graph.h"

Graph::Graph()
{
}

Graph::Graph(int count)
{
	for (int i = 0; i < count; i++) {
		task.push_back(new Node(i));
	}
}

Graph::~Graph()
{
	for (int i = 0; i < task.size(); i++) {
		delete task[i];
	}
}

void Graph::insert(int p, int q)
{
	task[p]->adjacent.push_back(task[q]);
	task[p]->degree += 1;

	task[q]->adjacent.push_back(task[p]);
	task[q]->degree += 1;
}

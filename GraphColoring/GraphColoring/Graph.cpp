#include "Graph.h"

Graph::Graph()
	: color_num(0)
{
}

Graph::Graph(int num_of_node)
	: color_num(0)
{
	for (int i = 0; i < num_of_node; i++) {
		task.push_back(new Node(i, num_of_node));
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

void Graph::distribute_task_to_thread(vector<TCB*>& tcb)
{
	const int mod = tcb.size();

	vector<Node*> tmp(task.begin(), task.end());
	sort(tmp.begin(), tmp.end(), Node::compare_by_priority);

	for (int i = 0; i < tmp.size(); i++) {
		int index = i % mod;
		tcb[index]->task.push_back(tmp[i]);
	}
}

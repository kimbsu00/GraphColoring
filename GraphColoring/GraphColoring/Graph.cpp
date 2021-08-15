#include "Graph.h"

Graph::Graph()
{
}

Graph::Graph(int num_of_node)
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

	for (int i = 0; i < task.size(); i++) {
		int index = task[i]->index % mod;
		task[i]->t_num = index;
		tcb[index]->task.push_back(task[i]);
	}
}

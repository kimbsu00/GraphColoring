#pragma once
#include <vector>
#include <algorithm>
#include "Node.h"
#include "TCB.h"

using namespace std;

class Graph
{
public:
	// degree�� �������� �������� ���ĵǾ� �ִ� Node���� ����
	vector<Node*> task;

	Graph();
	Graph(int num_of_node);
	~Graph();

	void insert(int p, int q);
	void distribute_task_to_thread(vector<TCB*>& tcb);
};


#pragma once
#include <vector>
#include <algorithm>
#include "Node.h"
#include "TCB.h"

using namespace std;

class Graph
{
public:
	// index�� �������� �������� ���ĵǾ� �ִ� Node���� ����
	vector<Node*> task;

	Graph();
	Graph(int num_of_node);
	~Graph();

	/*
	* ���� �ٸ� Node p, q�� �����ϴ� Edge�� �׷����� �߰��Ѵ�.
	*/
	void insert(int p, int q);

	/*
	* task �迭�� ���Ե� Node�� thread���� �й��Ѵ�.
	*/
	void distribute_task_to_thread(vector<TCB*>& tcb);
};


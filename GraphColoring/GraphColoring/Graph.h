#pragma once
#include <vector>
#include <algorithm>
#include "Node.h"
#include "TCB.h"

using namespace std;

class Graph
{
public:
	// degree를 기준으로 내림차순 정렬되어 있는 Node들의 집합
	vector<Node*> task;

	Graph();
	Graph(int num_of_node);
	~Graph();

	void insert(int p, int q);
	void distribute_task_to_thread(vector<TCB*>& tcb);
};


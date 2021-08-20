#pragma once
#include <vector>
#include <algorithm>
#include "Node.h"
#include "TCB.h"

using namespace std;

class Graph
{
public:
	// index를 기준으로 오름차순 정렬되어 있는 Node들의 집합
	vector<Node*> task;

	Graph();
	Graph(int num_of_node);
	~Graph();

	/*
	* 서로 다른 Node p, q를 연결하는 Edge를 그래프에 추가한다.
	*/
	void insert(int p, int q);

	/*
	* task 배열에 포함된 Node를 thread에게 분배한다.
	*/
	void distribute_task_to_thread(vector<TCB*>& tcb);
};


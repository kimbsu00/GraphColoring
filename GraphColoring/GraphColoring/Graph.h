#pragma once
#include <vector>
#include "Node.h"

using namespace std;

class Graph
{
public:
	// degree를 기준으로 내림차순 정렬되어 있는 Node들의 집합
	vector<Node*> task;

	Graph();
	Graph(int count);
	~Graph();

	void insert(int p, int q);
};


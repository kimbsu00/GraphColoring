#pragma once
#include <vector>
#include "Node.h"

using namespace std;

class Graph
{
public:
	// degree�� �������� �������� ���ĵǾ� �ִ� Node���� ����
	vector<Node*> task;

	Graph();
	Graph(int count);
	~Graph();

	void insert(int p, int q);
};


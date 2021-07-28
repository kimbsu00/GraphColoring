#pragma once
#include <vector>

using namespace std;

enum N_FLAG {
	CAN_SELECT,
	SELECTED,
	COLORED
};

class Node
{
public:
	// Node의 고유 번호, 0 이상의 정수
	int index;
	// 직접 연결되어 있는 Node의 수
	int degree;
	// 직접 연결되어 있는 노드들
	vector<Node*> adjacent;
	// 노드의 현재 상태를 의미함
	int n_flag;

	Node();
	Node(int index);
	Node(int index, int degree);
	~Node();

	bool check();
	bool operator <(const Node& other) const;
};


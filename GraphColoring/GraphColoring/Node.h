#pragma once
#include <vector>
#include <mutex>

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
	// Node의 현재 상태를 의미함
	int n_flag;
	// n_flag에 대한 mutex
	mutex n_flag_mutex;

	Node();
	Node(int index);
	Node(int index, int degree);
	~Node();

	bool check();
	void set_n_flag(int n_flag);
};


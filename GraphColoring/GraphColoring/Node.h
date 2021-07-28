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
	// Node�� ���� ��ȣ, 0 �̻��� ����
	int index;
	// ���� ����Ǿ� �ִ� Node�� ��
	int degree;
	// ���� ����Ǿ� �ִ� ����
	vector<Node*> adjacent;
	// ����� ���� ���¸� �ǹ���
	int n_flag;

	Node();
	Node(int index);
	Node(int index, int degree);
	~Node();

	bool check();
	bool operator <(const Node& other) const;
};


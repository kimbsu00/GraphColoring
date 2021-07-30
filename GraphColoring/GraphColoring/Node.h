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
	// Node�� ���� ��ȣ, 0 �̻��� ����
	int index;
	// ���� ����Ǿ� �ִ� Node�� ��
	int degree;
	// ���� ����Ǿ� �ִ� ����
	vector<Node*> adjacent;
	// Node�� ���� ���¸� �ǹ���
	int n_flag;
	// n_flag�� ���� mutex
	mutex n_flag_mutex;

	Node();
	Node(int index);
	Node(int index, int degree);
	~Node();

	bool check();
	void set_n_flag(int n_flag);
};


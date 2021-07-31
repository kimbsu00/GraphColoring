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
	// ���� ����Ǿ� �ִ� Node��
	vector<Node*> adjacent;
	// Node�� ���� ���¸� �ǹ���
	int n_flag;
	// n_flag�� ���� mutex
	mutex n_flag_mutex;
	// Node�� �Ҵ�� ���� �ǹ���, [0, Node����] ������ ����
	int color;
	// Node�� index��° ���� �Ҵ��� �� �ִ����� ��Ÿ��
	vector<bool> n_color;
	// n_color�� ���� mutex
	mutex n_color_mutex;

	Node();
	Node(int index);
	Node(int index, int num_of_node);
	~Node();

	bool check();
	void set_n_flag(int n_flag);
	void update_n_color(int already_used_color);

	static bool compare(const Node* a, const Node* b);
};


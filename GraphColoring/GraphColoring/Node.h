#pragma once
#include <vector>
#include <mutex>

using namespace std;

enum N_FLAG {
	CAN_SELECT,
	SELECTED,
	N_WAIT,
	COLORED
};

class Node
{
public:
	// Node의 고유 번호, 0 이상의 정수
	int index;
	// 직접 연결되어 있는 Node의 수
	int degree;
	// 직접 연결되어 있는 Node들
	vector<Node*> adjacent;
	// Node의 현재 상태를 의미함
	int n_flag;
	// n_flag에 대한 mutex
	mutex n_flag_mutex;
	// Node에 할당된 색을 의미함, [0, Node개수] 범위의 정수
	int color;
	// Node에 index번째 색을 할당할 수 있는지를 나타냄
	vector<bool> n_color;
	// n_color에 대한 mutex
	mutex n_color_mutex;
	// 해당 노드를 작업할 쓰레드 번호
	int t_num;
	// n_flag의 값을 N_FLAG::WAIT으로 변경했던 adjacent의 index를 의미함
	vector<int> restore_index;
	
	Node();
	Node(int index);
	Node(int index, int num_of_node);
	~Node();

	static bool compare(const Node* a, const Node* b);

	bool check();
	void set_n_flag(int n_flag);
	void update_n_color(int already_used_color);
	bool check_adjacent();
	void coloring();
};


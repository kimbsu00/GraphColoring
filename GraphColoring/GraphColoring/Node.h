#pragma once
#include <vector>
#include <mutex>
#include <omp.h>

using namespace std;

enum N_FLAG {
	UNCOLORED,
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
	// Node에 할당된 색을 의미함, [0, Node개수] 범위의 정수
	int color;
	// Node에 index번째 색을 할당할 수 있는지를 나타냄
	vector<bool> n_color;
	// n_color에 대한 lock
	omp_lock_t n_color_lock;
	
	Node();
	Node(int index);
	Node(int index, int num_of_node);
	~Node();

	static bool compare(const Node* a, const Node* b);

	/*
	* 인접한 노드들과 비교했을 때,
	* 우선순위가 제일 높으면 return true
	* 그렇지 않으면 return false
	*/
	bool is_priority();

	/*
	* Node를 색칠하고, 색칠한 결과를 인접한 Node에 반영한다.
	*/
	long long coloring();
};


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
	// 직접 연결되어 있는 Node들과 비교했을 때의 상대적 우선순위
	// 0이면 색칠할 수 있는 상태임을 의미한다.
	int priority;
	// priority에 대한 lock
	omp_lock_t priority_lock;
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

	/*
	* degree를 기준으로 Node를 비교하는 함수
	*/
	static bool compare(const Node* a, const Node* b);

	/*
	* priority를 기준으로 Node를 비교하는 함수
	*/
	static bool compare_by_priority(const Node* a, const Node* b);

	/*
	* 인접한 노드들과 비교했을 때,
	* 우선순위가 제일 높으면 return true
	* 그렇지 않으면 return false
	*/
	bool is_priority();

	/*
	* 인접한 노드들과 비교했을 때,
	* priority(상대적 우선순위)를 계산한다.
	* priority == 0 -> 색칠 가능한 경우를 의미
	* priority > 0  -> 먼저 색칠해야 하는 노드가 priority개 만큼 존재함을 의미
	*/
	void calculate_priority();

	/*
	* Node를 색칠하고, 색칠한 결과를 인접한 Node에 반영한다.
	*/
	long long coloring();
};


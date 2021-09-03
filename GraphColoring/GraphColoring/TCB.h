#pragma once
#include <vector>
#include <algorithm>
#include "Node.h"

using namespace std;

class TCB
{
public:
	// thread의 고유 번호, 0 이상의 정수
	int index;
	// thread가 색칠해야 하는 Node들
	vector<Node*> task;

	long long select_ref_count;
	long long coloring_ref_count;

	TCB();
	TCB(int index);
	~TCB();

	/*
	* task 배열에 존재하는 Node들 중에서,
	* thread가 색칠할 Node를 선택하고,
	* 선택한 Node의 index를 리턴한다.
	*/
	int select_task();
};


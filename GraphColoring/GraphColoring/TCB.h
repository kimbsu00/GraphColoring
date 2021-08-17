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

	TCB();
	TCB(int index);
	~TCB();

	int select_task();
};


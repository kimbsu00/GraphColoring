#pragma once
#include <vector>
#include <algorithm>
#include <mutex>
#include "Node.h"

using namespace std;

class TCB
{
public:
	// thread의 고유 번호, 0 이상의 정수
	int index;
	// thread의 상태를 의미함
	int t_flag;
	// t_flag에 대한 mutex
	mutex t_flag_mutex;
	// thread가 색칠해야 하는 Node들
	vector<Node*> task;

	TCB();
	TCB(int index);
	~TCB();
};


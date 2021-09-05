#pragma once
#include <vector>
#include <algorithm>
#include <ctime>
#include "Node.h"

using namespace std;

class TCB
{
public:
	// thread의 고유 번호, 0 이상의 정수
	int index;
	// thread가 색칠해야 하는 Node들
	vector<Node*> task;

	// select_task 함수에서 참조한 Node의 개수
	long long select_ref_count;
	// select_task 에서 모든 task에 대하여 select 작업이 실패한 횟수
	long long select_fail_count;
	// Node.coloring 함수에서 참조한 Node의 개수
	long long coloring_ref_count;
	clock_t running_time;

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


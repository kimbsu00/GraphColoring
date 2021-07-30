#pragma once
#include <mutex>

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

	TCB();
	TCB(int index);
	~TCB();
};


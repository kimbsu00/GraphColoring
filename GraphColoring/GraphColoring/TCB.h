#pragma once
#include <mutex>

using namespace std;

class TCB
{
public:
	// thread�� ���� ��ȣ, 0 �̻��� ����
	int index;
	// thread�� ���¸� �ǹ���
	int t_flag;
	// t_flag�� ���� mutex
	mutex t_flag_mutex;

	TCB();
	TCB(int index);
	~TCB();
};


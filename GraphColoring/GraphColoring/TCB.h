#pragma once
#include <vector>
#include <algorithm>
#include <mutex>
#include "Node.h"

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
	// thread�� ��ĥ�ؾ� �ϴ� Node��
	vector<Node*> task;

	TCB();
	TCB(int index);
	~TCB();
};


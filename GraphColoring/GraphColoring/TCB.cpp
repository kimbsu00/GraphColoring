#include "TCB.h"

TCB::TCB()
	: index(-1)
{
}

TCB::TCB(int index)
	: index(index)
{
}

TCB::~TCB()
{
}

int TCB::select_task()
{
	int ret = -1;
	int count = 0;

	for (int i = 0; i < task.size(); i++) {
		if (task[i]->n_flag == N_FLAG::UNCOLORED) {
			if (task[i]->is_priority()) {
				ret = i;
				break;
			}
		}
		else {
			count += 1;
		}

		if (i == task.size() - 1 && count != task.size()) {
			i = -1;
			count = 0;
			// 아 여기에 추가해 달라고요
		}
	}

	return ret;
}


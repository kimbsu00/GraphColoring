#include "TCB.h"

TCB::TCB()
	: index(-1), select_ref_count(0), coloring_ref_count(0), select_fail_count(0)
{
}

TCB::TCB(int index)
	: index(index), select_ref_count(0), coloring_ref_count(0), select_fail_count(0)
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
		select_ref_count++;
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
			select_fail_count++;
			i = -1;
			count = 0;
		}
	}

	return ret;
}


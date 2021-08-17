#include "TCB.h"

TCB::TCB()
	: index(-1), t_flag(SELECTING)
{
}

TCB::TCB(int index)
	: index(index), t_flag(SELECTING)
{
}

TCB::~TCB()
{
}


void TCB::set_t_flag(int t_flag){
	this->t_flag_mutex.lock();
	this->t_flag = t_flag;
	this->t_flag_mutex.unlock();
}

int TCB::select_task()
{
	int ret = -1;
	bool fin = true;

	for (int i = 0; i < task.size(); i++) {
		task[i]->n_flag_mutex.lock();
		if (task[i]->n_flag == N_FLAG::CAN_SELECT) {
			task[i]->n_flag = N_FLAG::SELECTED;
			fin = false;
			ret = i;
			task[i]->n_flag_mutex.unlock();
			break;
		}
		task[i]->n_flag_mutex.unlock();

		if (task[i]->n_flag == N_FLAG::N_WAIT) {
			fin = false;
		}

		if (i == task.size() - 1) {
			if (fin) {
				ret = -1;
				break;
			}
			else {
				i = -1;
			}
		}
	}
	return ret;
}

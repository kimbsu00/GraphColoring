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
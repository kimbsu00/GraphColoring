#include "Node.h"

Node::Node()
	: index(-1), degree(-1), n_flag(CAN_SELECT)
{
}

Node::Node(int index)
	: index(index), degree(-1), n_flag(CAN_SELECT)
{
}

Node::Node(int index, int degree)
	: index(index), degree(degree), n_flag(CAN_SELECT)
{
}

Node::~Node()
{
}

bool Node::check()
{
	for (int i = 0; i < adjacent.size(); i++) {
		if (adjacent[i]->n_flag == SELECTED) {
			return false;
		}
	}
	return true;
}

void Node::set_n_flag(int n_flag)
{
	this->n_flag_mutex.lock();
	this->n_flag = n_flag;
	this->n_flag_mutex.unlock();
}

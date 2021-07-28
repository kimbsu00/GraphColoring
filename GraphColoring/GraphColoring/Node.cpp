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

bool Node::operator<(const Node& other) const
{
	return this->degree < other.degree;
}

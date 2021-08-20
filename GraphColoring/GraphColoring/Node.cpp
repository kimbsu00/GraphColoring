#include "Node.h"

Node::Node()
	: index(-1), degree(0), n_flag(UNCOLORED), color(-1)
{
}

Node::Node(int index)
	: index(index), degree(0), n_flag(UNCOLORED), color(-1)
{
}

Node::Node(int index, int num_of_node)
	: index(index), degree(0), n_flag(UNCOLORED), color(-1)
{
	n_color.resize(num_of_node, true);
}

Node::~Node()
{
}

bool Node::compare(const Node* a, const Node* b)
{
	if (a->degree == b->degree)
		return a->index < b->index;
	return a->degree > b->degree;
}

bool Node::is_priority()
{
	bool ret = true;

	for (int i = 0; i < adjacent.size(); i++) {
		if (!compare(this, adjacent[i])) {
			ret = false;
			break;
		}
	}

	return ret;
}

void Node::coloring()
{
	int color = 0;
	for (; color < n_color.size(); color++) {
		if (n_color[color]) {
			this->color = color;
			break;
		}
	}

	for (int i = 0; i < adjacent.size(); i++) {
		adjacent[i]->n_color_mutex.lock();
		adjacent[i]->n_color[color] = false;
		adjacent[i]->n_color_mutex.unlock();

		adjacent[i]->degree_mutex.lock();
		if (adjacent[i]->n_flag == N_FLAG::UNCOLORED) {
			adjacent[i]->degree -= 1;
		}
		adjacent[i]->degree_mutex.unlock();
	}

	/*
	* this->degree�� adjacent[i]->degree���� ���� ������ ���,
	* ������ �� ��尡 ���� ���� ���� �� �� �ִ�.
	*/ 
	this->degree_mutex.lock();
	this->degree = -1;
	this->degree_mutex.unlock();

	this->n_flag = N_FLAG::COLORED;
}


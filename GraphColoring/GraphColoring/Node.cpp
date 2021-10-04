#include "Node.h"

Node::Node()
	: index(-1), degree(0), n_flag(UNCOLORED), color(-1), priority(-1)
{
	omp_init_lock(&priority_lock);
	omp_init_lock(&n_color_lock);
}

Node::Node(int index)
	: index(index), degree(0), n_flag(UNCOLORED), color(-1), priority(-1)
{
	omp_init_lock(&priority_lock);
	omp_init_lock(&n_color_lock);
}

Node::Node(int index, int num_of_node)
	: index(index), degree(0), n_flag(UNCOLORED), color(-1), priority(-1)
{
	omp_init_lock(&priority_lock);
	omp_init_lock(&n_color_lock);
	n_color.resize(num_of_node, true);
}

Node::~Node()
{
	omp_destroy_lock(&priority_lock);
	omp_destroy_lock(&n_color_lock);
}

bool Node::compare(const Node* a, const Node* b)
{
	if (a->degree == b->degree)
		return a->index < b->index;
	return a->degree > b->degree;
}

bool Node::compare_by_priority(const Node* a, const Node* b)
{
	if (a->priority == b->priority)
		return a->index < b->index;
	return a->priority < b->priority;
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

void Node::calculate_priority()
{
	int tmp = 0;

	for (int i = 0; i < adjacent.size(); i++) {
		if (!compare(this, adjacent[i]))			tmp += 1;
	}
	this->priority = tmp;
}

long long Node::coloring()
{
	int color = 0;
	for (; color < n_color.size(); color++) {
		if (n_color[color]) {
			this->color = color;
			break;
		}
	}

	for (int i = 0; i < adjacent.size(); i++) {
		omp_set_lock(&(adjacent[i]->n_color_lock));
		adjacent[i]->n_color[color] = false;
		omp_unset_lock(&(adjacent[i]->n_color_lock));
	}

	for (int i = 0; i < adjacent.size(); i++) {
		if (adjacent[i]->n_flag == N_FLAG::COLORED)			continue;

		omp_set_lock(&(adjacent[i]->priority_lock));
		adjacent[i]->priority -= 1;
		omp_unset_lock(&(adjacent[i]->priority_lock));
	}

	this->degree = -1;
	this->priority = -1;

	this->n_flag = N_FLAG::COLORED;

	return adjacent.size();
}


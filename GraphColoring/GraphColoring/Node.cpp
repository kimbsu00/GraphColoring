#include "Node.h"

Node::Node()
	: index(-1), degree(0), n_flag(UNCOLORED), color(-1)
{
	omp_init_lock(&degree_lock);
	omp_init_lock(&n_color_lock);
}

Node::Node(int index)
	: index(index), degree(0), n_flag(UNCOLORED), color(-1)
{
	omp_init_lock(&degree_lock);
	omp_init_lock(&n_color_lock);
}

Node::Node(int index, int num_of_node)
	: index(index), degree(0), n_flag(UNCOLORED), color(-1)
{
	omp_init_lock(&degree_lock);
	omp_init_lock(&n_color_lock);
	n_color.resize(num_of_node, true);
}

Node::~Node()
{
	omp_destroy_lock(&degree_lock);
	omp_destroy_lock(&n_color_lock);
}

bool Node::compare(const Node* a, const Node* b)
{
	if (a->degree == b->degree)
		return a->index < b->index;
	return a->degree < b->degree;
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

		omp_set_lock(&(adjacent[i]->degree_lock));
		if (adjacent[i]->n_flag == N_FLAG::UNCOLORED) {
			adjacent[i]->degree -= 1;
		}
		omp_unset_lock(&(adjacent[i]->degree_lock));
	}

	/*
	* this->degree를 adjacent[i]->degree보다 먼저 변경할 경우,
	* 인접한 두 노드가 서로 같은 색이 될 수 있다.
	*/ 
	omp_set_lock(&degree_lock);
	this->degree = 1987654321;
	omp_unset_lock(&degree_lock);

	this->n_flag = N_FLAG::COLORED;

	return adjacent.size();
}


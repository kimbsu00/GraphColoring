#include "Node.h"

Node::Node()
	: index(-1), degree(0), n_flag(CAN_SELECT), color(-1), t_num(-1)
{
}

Node::Node(int index)
	: index(index), degree(0), n_flag(CAN_SELECT), color(-1), t_num(-1)
{
}

Node::Node(int index, int num_of_node)
	: index(index), degree(0), n_flag(CAN_SELECT), color(-1), t_num(-1)
{
	n_color.resize(num_of_node, true);
}

Node::~Node()
{
}

bool Node::compare(const Node* a, const Node* b)
{
	return a->degree > b->degree;
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

void Node::update_n_color(int already_used_color)
{
	this->n_color_mutex.lock();
	this->n_color[already_used_color] = false;
	this->n_color_mutex.unlock();
}

bool Node::check_adjacent()
{
	bool ret = true;
	restore_index.clear();

	for (int i = 0; i < adjacent.size(); i++) {
		if (adjacent[i]->n_flag == N_FLAG::SELECTED) {
			for (int j = 0; j < restore_index.size(); j++) {
				adjacent[i]->set_n_flag(N_FLAG::CAN_SELECT);
			}
			ret = false;
			break;
		}
		
		adjacent[i]->n_flag_mutex.lock();
		if (adjacent[i]->n_flag == N_FLAG::CAN_SELECT) {
			adjacent[i]->n_flag = N_FLAG::N_WAIT;
			restore_index.push_back(i);
		}
		adjacent[i]->n_flag_mutex.unlock();
	}

	return ret;
}

void Node::coloring()
{
	int color = 0;
	n_color_mutex.lock();
	for (; color < n_color.size(); color++) {
		if (n_color[color]) {
			this->color = color;
			break;
		}
	}
	n_color_mutex.unlock();
	set_n_flag(N_FLAG::COLORED);

	for (int i = 0; i < adjacent.size(); i++) {
		adjacent[i]->update_n_color(color);
	}

	for (int i = 0; i < restore_index.size(); i++) {
		adjacent[restore_index[i]]->set_n_flag(N_FLAG::CAN_SELECT);
	}
}

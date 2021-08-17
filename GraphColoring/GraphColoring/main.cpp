#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Graph.h"
#include "TCB.h"

#define MAX_THREAD_NUM 4

using namespace std;

// array of thread control block
vector<TCB*> tcb;

// Input Graph
Graph* graph = nullptr;

vector<string> split(string& input, char delimiter) {
	vector<string> answer;
	stringstream ss(input);
	string temp;

	while (getline(ss, temp, delimiter)) {
		answer.push_back(temp);
	}

	return answer;
}

bool make_graph() {
	ifstream ifs;
	ifs.open("data\\test3.txt");

	if (ifs.fail())			return false;
	
	string str;
	getline(ifs, str);
	vector<string> temp = split(str, ' ');
	int num_of_node = stoi(temp[2]);
	int num_of_edge = stoi(temp[3]);

	graph = new Graph(num_of_node);

	while (getline(ifs, str)) {
		if (str.empty())			break;

		temp = split(str, ' ');
		int p = stoi(temp[1]) - 1;
		int q = stoi(temp[2]) - 1;
		graph->insert(p, q);
	}

	return true;
}

bool prove() {
	ifstream ifs;
	ifs.open("data\\output3.txt");

	if (ifs.fail())			return false;

	string str;
	int index = 0;
	while (getline(ifs, str)) {
		if (str.empty())			break;

		graph->task[index]->color = stoi(str);
		index++;
	}

	bool ret = true;
	for (int i = 0; i < graph->task.size(); i++) {
		Node* task = graph->task[i];
		for (int j = 0; j < task->adjacent.size(); j++) {
			Node* adj = graph->task[i]->adjacent[j];
			if (task->color == adj->color) {
				ret = false;
				break;
			}
		}
	}
	return ret;
}

void thread_work(int thread_idx) {
	// thread 별로 가지고 있는 task를 degree 기준으로 내림차순 정렬함.
	sort(tcb[thread_idx]->task.begin(), tcb[thread_idx]->task.end(), Node::compare);

	int node_idx = -1;
	while ((node_idx = tcb[thread_idx]->select_task()) != -1) {
		Node* node = tcb[thread_idx]->task[node_idx];
		node->coloring();
	}
}

int main(void) {
	if (!make_graph()) {
		cout << "test file is not open.\n";
		return 0;
	}
	
	//bool prove_ret = prove();

	vector<thread*> threads(MAX_THREAD_NUM, nullptr);
	for (int i = 0; i < threads.size(); i++) {
		tcb.push_back(new TCB(i));
	}
	graph->distribute_task_to_thread(tcb);

	for (int i = 0; i < threads.size(); i++) {
		threads[i] = new thread(thread_work, i);
	}

	for (int i = 0; i < threads.size(); i++) {
		threads[i]->join();
	}

	// test code
	//for (int i = 0; i < graph->task.size(); i++) {
	//	//cout << "\n\nNode #" << graph->task[i]->index << "\n";
	//	cout << graph->task[i]->color << "\n";
	//}

	return 0;
}
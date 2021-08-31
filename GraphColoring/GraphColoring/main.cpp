#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <windows.h>
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

bool make_graph(int data_index) {
	string fileName = "data\\input\\test";
	fileName.append(to_string(data_index));
	fileName.append(".txt");

	ifstream ifs;
	ifs.open(fileName);

	if (ifs.fail())			return false;
	
	string str;
	vector<string> temp;

	while (getline(ifs, str)) {
		if (str.empty())			break;

		temp = split(str, ' ');
		if (temp[0].compare("p") == 0) {
			int num_of_node = stoi(temp[2]);
			int num_of_edge = stoi(temp[3]);
			graph = new Graph(num_of_node);
		}
		else if (temp[0].compare("e") == 0) {
			int p = stoi(temp[1]) - 1;
			int q = stoi(temp[2]) - 1;
			graph->insert(p, q);
		}
	}

	return true;
}

void make_output(int data_index) {
	string fileName = "data\\output\\output";
	fileName.append(to_string(data_index));
	fileName.append(".txt");

	ofstream ofs;
	ofs.open(fileName);

	for (int i = 0; i < graph->task.size(); i++) {
		string str = to_string(graph->task[i]->color);
		str.append(1, '\n');
		ofs.write(str.c_str(), str.size());
	}
	ofs.close();
}

bool prove(int data_index) {
	string fileName = "data\\output\\output";
	fileName.append(to_string(data_index));
	fileName.append(".txt");

	ifstream ifs;
	ifs.open(fileName);

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
	/*
	* range of data_index is [1, 22].
	*/
	const int data_index = 21;

	if (!make_graph(data_index)) {
		cout << "test file is not open.\n";
		return 0;
	}

	vector<thread*> threads(MAX_THREAD_NUM, nullptr);
	for (int i = 0; i < threads.size(); i++) {
		tcb.push_back(new TCB(i));
	}
	graph->distribute_task_to_thread(tcb);

	ULONGLONG dw_start = GetTickCount64();

	for (int i = 0; i < threads.size(); i++) {
		threads[i] = new thread(thread_work, i);
	}

	for (int i = 0; i < threads.size(); i++) {
		threads[i]->join();
	}

	cout << GetTickCount64() - dw_start << " millisecond\n";

	make_output(data_index);

	bool prove_ret = prove(data_index);
	cout << "prove value = " << (prove_ret ? "true" : "false") << "\n";

	return 0;
}
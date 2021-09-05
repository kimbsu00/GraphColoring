#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <omp.h>
#include <ctime>
#include <set>
#include "Graph.h"
#include "TCB.h"

#define MAX_THREAD_NUM 8

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
	set<int> colors;
	for (int i = 0; i < graph->task.size(); i++) {
		colors.insert(graph->task[i]->color);
	}
	graph->color_num = colors.size();

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
	string str = "used color num : ";
	str.append(to_string(graph->color_num));
	str.append(1, '\n');
	ofs.write(str.c_str(), str.size());
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
		if (str[0] == 'u')			break;

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
	clock_t thread_start = clock();

	TCB* m_tcb;
	#pragma omp critical 
	 m_tcb = tcb[thread_idx];

	// thread 별로 가지고 있는 task를 degree 기준으로 내림차순 정렬함.
	sort(m_tcb->task.begin(), m_tcb->task.end(), Node::compare);

	int node_idx = -1;
	while ((node_idx = m_tcb->select_task()) != -1) {
		Node* node = m_tcb->task[node_idx];
		m_tcb->coloring_ref_count += node->coloring(); 
	}

	clock_t thread_end = clock();
	m_tcb->running_time = thread_end - thread_start;
	//#pragma omp critical
	//cout << "thread_idx is " << thread_idx << " and time is " << m_tcb->running_time << " ms\n";
}

int main(void) {
	/*
	* range of data_index is [1, 22].
	*/
	const int data_index = 17;

	if (!make_graph(data_index)) {
		cout << "test file is not open.\n";
		return 0;
	}
	//bool prove_ret = prove(data_index);

	for (int i = 0; i < MAX_THREAD_NUM; i++) {
		tcb.push_back(new TCB(i));
	}
	graph->distribute_task_to_thread(tcb);

	clock_t start_time = clock();

	#pragma omp parallel num_threads(MAX_THREAD_NUM)
	{
		int thread_idx = omp_get_thread_num();
		thread_work(thread_idx);
	}

	#pragma omp barrier
	clock_t end_time = clock();
	cout << end_time - start_time << "ms\n";

	make_output(data_index);

	bool prove_ret = prove(data_index);
	cout << "prove value = " << (prove_ret ? "true" : "false") << "\n";

	for (int i = 0; i < tcb.size(); i++) {
		cout << "thread_idx is " << tcb[i]->index << " and time is " << tcb[i]->running_time << "ms\n";
	}

	return 0;
}
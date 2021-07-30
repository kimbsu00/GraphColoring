#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <sstream>
#include "Graph.h"
#include "TCB.h"

#define MAX_THREAD_NUM 16

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
	ifs.open("data\\test1.txt");

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

	graph->sort_task();
}

void coloring(int index) {
	// test code
	tcb[index]->t_flag_mutex.lock();
	cout << tcb[index]->t_flag << "\n";
	tcb[index]->t_flag_mutex.unlock();
}

int main(void) {
	if (!make_graph()) {
		cout << "test file is not open.\n";
		return 0;
	}

	vector<thread*> threads(MAX_THREAD_NUM, nullptr);
	for (int i = 0; i < threads.size(); i++) {
		tcb.push_back(new TCB(i));
		threads[i] = new thread(coloring, i);
	}

	for (int i = 0; i < threads.size(); i++) {
		threads[i]->join();
	}

	return 0;
}
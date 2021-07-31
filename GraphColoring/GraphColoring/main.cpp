#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
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

	return true;
}

void coloring() {

}

void selecting(int index) {
	sort(tcb[index]->task.begin(), tcb[index]->task.end(), Node::compare);


}

int main(void) {
	if (!make_graph()) {
		cout << "test file is not open.\n";
		return 0;
	}

	vector<thread*> threads(MAX_THREAD_NUM, nullptr);
	for (int i = 0; i < threads.size(); i++) {
		tcb.push_back(new TCB(i));
	}
	graph->distribute_task_to_thread(tcb);

	for (int i = 0; i < threads.size(); i++) {
		threads[i] = new thread(selecting, i);
	}

	for (int i = 0; i < threads.size(); i++) {
		threads[i]->join();
	}

	// test code
	for (int i = 0; i < tcb.size(); i++) {
		cout << "\n\nTCB #" << i << "\n";
		for (int j = 0; j < tcb[i]->task.size(); j++) {
			cout << tcb[i]->task[j]->degree << "\n";
		}
	}

	return 0;
}
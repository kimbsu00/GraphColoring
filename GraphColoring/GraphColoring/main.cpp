#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <omp.h>
#include <ctime>
#include <set>
#include <iomanip>
#include "Graph.h"
#include "TCB.h"

#define MAX_THREAD_NUM 8

using namespace std;

// array of thread control block
vector<TCB*> tcb;

// Input Graph
Graph* graph = nullptr;

/*
* delimeter 기준으로 input 문자열을 split하는 함수
* 나누어진 문자열을 vector<string> 타입으로 리턴한다.
*/
vector<string> split(string& input, char delimiter) {
	vector<string> answer;
	stringstream ss(input);
	string temp;

	while (getline(ss, temp, delimiter)) {
		answer.push_back(temp);
	}

	return answer;
}

/*
* file_name에 위치한 .txt 파일로부터 데이터를 읽고, 그래프를 생성하는 함수
*/
bool make_graph(string file_name) {
	ifstream ifs;
	ifs.open(file_name);

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

/*
* data\output\output.txt 경로에 output을 저장하는 함수
*/
void make_output(string last_name) {
	set<int> colors;
	for (int i = 0; i < graph->task.size(); i++) {
		colors.insert(graph->task[i]->color);
	}
	graph->color_num = colors.size();

	string file_name = "data\\output\\";
	file_name.append(last_name);

	ofstream ofs;
	ofs.open(file_name);

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

	cout << "\nThe path of output data's directory\n";
	cout << "..\\GraphColoring\\GraphColoring\\GraphColoring\\data\\output\\" << last_name << "\n";
}

/*
* 색칠된 그래프에 대하여, 인접한 노드가 서로 다른 색을 갖는지 확인하는 함수
* 인접한 노드끼리 다른 색을 갖는 경우에는 return true
* 인접한 노드끼리 같은 색을 갖는 경우에는 return false
*/
bool prove(string last_name) {
	string file_name = "data\\output\\";
	file_name.append(last_name);

	ifstream ifs;
	ifs.open(file_name);

	if (ifs.fail())			return false;

	string str;
	int index = 0;
	while (getline(ifs, str)) {
		if (str.empty())			break;
		if (str[0] == 'u')			break;

		graph->task[index]->color = stoi(str);
		index++;
	}

	for (int i = 0; i < graph->task.size(); i++) {
		Node* task = graph->task[i];
		for (int j = 0; j < task->adjacent.size(); j++) {
			Node* adj = graph->task[i]->adjacent[j];
			if (task->color == adj->color) {
				return false;
			}
		}
	}

	return true;
}

/*
* Thread가 수행하는 함수
* 색칠할 Node를 선택하고, 색칠하는 과정을 반복한다.
*/
void thread_work(int thread_idx) {
	clock_t thread_start = clock();

	TCB* m_tcb;
	//#pragma omp critical 
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
}

int main(void) {
	string file_name = "";
	cout << "Enter the input data's absolute path: ";
	cin >> file_name;

	if (!make_graph(file_name)) {
		cout << "Input file is not open.\n";
		return 0;
	}

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

	cout << left;
	cout << "\nTotal Runtime\t" << end_time - start_time << "ms\n";
	for (int i = 0; i < tcb.size(); i++) {
		cout << setw(10) << "Thread" << setw(5) << tcb[i]->index << setw(10) << "Runtime" << tcb[i]->running_time << "ms\n";
	}

	vector<string> tmp = split(file_name, '\\');
	make_output(tmp.back());

	if (prove(tmp.back())) {
		cout << "\nThere is no pair of node that has same color.\n";
	}
	else {
		cout << "\nThere is pair of node that has same color.\n";
	}

	return 0;
}
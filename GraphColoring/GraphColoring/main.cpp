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

void t_work(int t_num) {
	
	// init
	// vector<*Node>* t_task = tcb[t_num]->task; 이렇게 바꿔도 되려나??
	sort(tcb[t_num]->task.begin(), tcb[t_num]->task.end(), Node::compare);
	int tmp_flag;
	
	// select
	tcb[t_num]->task[index]->set_n_flag(SELETING);
	int index=0;
	bool fin=true;
	for(index =0; index<tcb[t_num]->task.size(); index++){
		tmp_flag = tcb[t_num]->task[index]->n_flag; 
		if(tmp_flag==CAN_SELECT){
			fin=false;
			break;
		}
		if(tmp_flag==WAIT)
			fin=false;
		
		if(index==(tcb[t_num]->task.size()-1)){
			if(fin){
				// 쓰레드 종료
				tcb[t_num]->set_t_flag(DONE);
				
			}else{
				index = -1;
			}
		}
	}
	tcb[t_num]->task[index]->set_n_flag(SELECTED);
	tcb[t_num]->set_t_flag(WAIT);
	
	// wait
	bool check = true;
	for(int i=0; i<tcb[t_num]->task[index]->adjacent.size();i++){
		Node* adj = tcb[t_num]->task[index]->adjacent[i];
		tmp_flag = tcb[adj->t_num]->t_flag;
			
		// 여기부분이 쪼금 맘에 안들긴한다.... 아이디어있으면 알려주셈
		// 해당 쓰레드만 다시 검사할수있는 방법이 있으면 좋을거같긴함,,,
		while(tmp_flag == SELETING)
			tmp_flag = tcb[adj->t_num]->t_flag;
			
		if(tmp_flag == COLORING){
			if(adj->n_flag == SELECTED){
				check = false;
				break;
			}
			if(adj->n_flag == CAN_SELECT){
				adj->set_n_flag(WAIT);
				// 문제가능성있음
				// WAIT으로 바꾸기 이전에 해당 쓰레드가 coloring을 마치고 해당 노드를 선택
			}
		}
	}
	
	if(check)
		// coloring
	else
		// select로 복귀
	
	// coloring
	tcb[t_num]->set_t_flag(COLORING);
	int tmp_color=0;
	for(;tmp_color<tcb[t_num].task[index]->n_color.size(); tmp_color++){
		if(tcb[t_num].task[index]->n_color[tmp_color])
			break;
	}
	tcb[t_num].task[index]->color = tmp_color;
	for(int i=0; i<tcb[t_num].task[index]->adjacent.size(); i++){
		tcb[t_num].task[index]->adjacent[i].n_color[tmp_color] = false;
		// adjacent에서 제거하는방법을 너가 말한대로 배열하나 쓰는게 좋을듯 ㅋㅋ
		// 
	}
	
	
	
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
		threads[i] = new thread(t_work, i);
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
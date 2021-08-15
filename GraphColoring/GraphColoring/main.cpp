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


int selecting(int t_num){
	tcb[t_num]->set_t_flag(SELETING);
	
	int tmp_flag;
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
				index = -1;
				break;
			}else{	// 아직 선택할 노드가 남아있으면 처음부터 다시 탐색
				index = -1;
			}
		}
	}
	return index;
}

int waitting(int t_num, int index){
	
	
	tcb[t_num]->task[index]->set_n_flag(SELECTED);
	tcb[t_num]->set_t_flag(WAIT);
	int tmp_flag;
	// wait
	int next = COLORING;
	for(int i=0; i<tcb[t_num]->task[index]->adjacent.size();i++){
		Node* adj = tcb[t_num]->task[index]->adjacent[i];
		tmp_flag = tcb[adj->t_num]->t_flag;
			
		// 여기부분이 쪼금 맘에 안들긴한다.... 아이디어있으면 알려주셈
		// 해당 쓰레드만 다시 검사할수있는 방법이 있으면 좋을거같긴함,,,
		if(tmp_flag == SELETING)
			next = WAIT;
			
		if(tmp_flag == COLORING){
			if(adj->n_flag == SELECTED){
				next = SELECTING;
				break;
			}
			if(adj->n_flag == CAN_SELECT){
				adj->set_n_flag(WAIT);
				// 문제가능성있음
				// WAIT으로 바꾸기 이전에 해당 쓰레드가 coloring을 마치고 해당 노드를 선택
				// 혹은 현재쓰레드가 경쟁에서 밀릴 가능성,,,
			}
		}
		if(i==tcb[t_num]->task[index]->adjacent.size()-1){
			if(next==WAIT){
				next==COLORING;
				i=-1;
			}
			else
				break;
		}
	}
	return next;
}

void coloring() {

	
	
	tcb[t_num]->set_t_flag(COLORING);
	int tmp_color=0;
	
	// 선택한 node의 색상칠하는 과정 - n_color에서 가장먼저 true 인 지점의 index를 color값으로 지정
	for(;tmp_color<tcb[t_num].task[index]->n_color.size(); tmp_color++){
		if(tcb[t_num].task[index]->n_color[tmp_color])
			tcb[t_num].task[index]->set_n_flag(COLORED);
			break;
	}
	tcb[t_num].task[index]->color = tmp_color;
	
	// 선택한 node의 adjacent들의 n_color 수정 및 adjacent에서 node 제거.
	for(int i=0; i<tcb[t_num].task[index]->adjacent.size(); i++){
		tcb[t_num].task[index]->adjacent[i]->n_color[tmp_color] = false;		// n_color 변경
		int j=0;
		
		// adjacent의 adjacent 에서 선택된 node 제거하는 과정..
		while(1){
			if(tcb[t_num].task[index]->adjacent[i]->adjacent[j]->index == tcb[t_num].task[index]->adjacent[i]->index){
				// 해당 node가 선택된 node 때문에 WAIT이 걸렸을수 있기 때문에 WAIT인경우 풀어준다.
				if(tcb[t_num].task[index]->adjacent[i]->adjacent[j]->n_flag==WAIT){
					tcb[t_num].task[index]->adjacent[i]->adjacent[j]->set_n_flag(CAN_SELECT);
				}
					
				tcb[t_num].task[index]->adjacent[i]->adjacent.erase(tcb[t_num].task[index]->adjacent[i]->adjacent.begin()+j);
				break;
			}
			j++;
		}
	}
}

void t_work(int t_num) {
	
	// init
	// vector<*Node>* t_task = tcb[t_num]->task; 이렇게 바꿔도 되려나??
	sort(tcb[t_num]->task.begin(), tcb[t_num]->task.end(), Node::compare);
	
	
	// 각 함수는 t_flag가 변경되는 기준으로 만들어짐
	// 각 함수에서 n_flag가 변경될때 해당부분에서 문제가 생길가능성이없도록 만들면될듯,,,
	// 선택된 node가 붙어있는 경우 따로 처리를 안함 추가해야함 원래는 coloring에 넣으려구 했는데, 지금보니까 그냥 wait에서 처리해도 될랑가싶기두하고,,
	while(1){
		// select
		int node_index;	// select하고 선택한 node의 index값
		node_index = selecting(int t_num);
		if(node_index == -1;){
			// 쓰레드 종료
			
			break;
		}
		if(waitting(t_num, node_index)==SELECTING)
			continue;
		//if(next==COLORING)
		coloring(t_num, node_index);
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
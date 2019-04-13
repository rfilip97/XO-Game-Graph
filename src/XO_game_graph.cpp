//============================================================================
// Name        : XO_game_graph.cpp
// Author      : Razvan Filip
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

using namespace std;

int check_win_condition(std::string game_state){

	// 3 in a line
	std::regex re("\\|([x0]?)\\1{2}\\|");
	std::smatch match;

	if (std::regex_search(game_state, match, re) && match.size() > 1){
		return 1;
	}

	// 3 in a row
	std::regex re2(".*(\\|([x0]).*\\|\\2.*\\|\\2)|(([x0])\\|.*\\4\\|.*\\4\\|)|(\\|.+([x0]).+\\|.+\\6.+\\|.+\\6.+)\\|.*");
	std::smatch match2;

	if (std::regex_search(game_state, match2, re2) && match2.size() > 1){
		return 1;
	}


	// diagonals
	std::regex re3("\\|([x0]).*\\|.+\\1.+\\|.+.+\\1\\||\\|.*([x0])\\|.+\\2.+\\|\\2.+.+\\|");
	std::smatch match3;

	if (std::regex_search(game_state, match3, re3) && match3.size() > 1){
		return 1;
	}

	return 0;
}

class Graph {

	public:
	Graph(std::string initial_state) {
		state = initial_state;
	};


	std::string state = "";
	Graph *next_graph[12] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
	int next_graph_current_index = 0;

	int index_real_to_logical(int pos) {

		switch(pos){

			case 1 : return 0;
			case 2 : return 1;
			case 3 : return 2;
			case 5 : return 3;
			case 6 : return 4;
			case 7 : return 5;
			case 9 : return 6;
			case 10 : return 7;
			case 11 : return 8;
			default : return -1;

		}
	}

	/*
	 *  From the state member, return the index
	 * of the next free position(marked as '-'),
	 * or -1 if table is full
	 */
	int get_next_free_pos(int current_pos) {

		if(current_pos >= 12)
			return 0;

		for(int i = current_pos; i < 12; i++) {
			if(state.at(i) == '-'){
				return i;
			}
		}

		return 0;
	}

};

/* 1 -> X, 0 -> 0 */
int turn = 1;

/* Keeps track of current player  */
std::string current_piece(){

	turn = 1 - turn;

	return turn == 0 ? "X" : "0";
}

int generate_game_graph(Graph *graph, std::string piece){

	int index = graph->get_next_free_pos(0);

	piece = (piece == "X") ? "0" : "X";

	while(index != 0){

		Graph *node = new Graph(graph->state);

		node->state.replace(index, 1, piece);
		graph->next_graph[graph->next_graph_current_index] = node;

		generate_game_graph(graph->next_graph[graph->next_graph_current_index++], piece);

		index = graph->get_next_free_pos(index + 1);
	}

	return 0;
}

ofstream myfile;

void printLevel(Graph graph, int desired, int current){

	if(desired == current){

		if(graph.next_graph_current_index > 0){
			myfile << "\n" << graph.state << " -> ";

			for(int i = 0; i < graph.next_graph_current_index; i++) {
				myfile << graph.next_graph[i]->state << " ";
			}

		}
	} else {
		for(int i = 0; i < graph.next_graph_current_index; i++) {
			printLevel(*graph.next_graph[i], desired, current + 1);
		}
	}
}

void print(Graph graph){

	for(int i = 0; i < 9; i++) {
		myfile << "\n Level: " << i << "\n";
		printLevel(graph, i, 0);
	}
}

int main() {

	Graph graph("|---|---|---|");

	myfile.open ("results.txt");
	generate_game_graph(&graph, "0");
	print(graph);

	myfile.close();

	return 0;
}

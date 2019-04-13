//============================================================================
// Name        : XO_game_graph.cpp
// Author      : Razvan Filip
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <regex>

using namespace std;

int check_win_condition(std::string game_state){

	// 3 in a line
	std::regex re("\\|([x0]?)\\1{2}\\|");
	std::smatch match;

	if (std::regex_search(game_state, match, re) && match.size() > 1){
		std::cout << "yes\n";
		return 1;
	}

	// 3 in a row
	std::regex re2(".*(\\|([x0]).*\\|\\2.*\\|\\2)|(([x0])\\|.*\\4\\|.*\\4\\|)|(\\|.+([x0]).+\\|.+\\6.+\\|.+\\6.+)\\|.*");
	std::smatch match2;

	if (std::regex_search(game_state, match2, re2) && match2.size() > 1){
		std::cout << "yes2\n";
		return 1;
	}


	// diagonals
	std::regex re3("\\|([x0]).*\\|.+\\1.+\\|.+.+\\1\\||\\|.*([x0])\\|.+\\2.+\\|\\2.+.+\\|");
	std::smatch match3;

	if (std::regex_search(game_state, match3, re3) && match3.size() > 1){
		std::cout << "yes3\n";
		return 1;
	}

	std::cout << "nope\n";
	return 0;
}

int main() {
	std::string game_state = "|x00|00x|0xx|";
	check_win_condition(game_state);
	return 0;
}

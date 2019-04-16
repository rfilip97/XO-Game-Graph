//============================================================================
// Name        : XO_game_tree.cpp
// Author      : Razvan Filip
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <regex>


/**
 * Check if the game state contains a win pattern, using regex.
 * This is divided in 3 parts, each searching for a specific
 * pattern: 3 in a row, 3 in a line or full diagonal.
 *
 * @return 0 if win pattern found, 1 otherwise
 */
int check_win_condition(std::string game_state){

	// 3 in a line
	std::regex re(".*\\|([X0]?)\\1{2}\\|.*");
	std::smatch match;

	if (std::regex_search(game_state, match, re) && match.size() > 1){
		return 0;
	}

	// 3 in a row
	std::regex re2(".*(\\|([X0]).*\\|\\2.*\\|\\2)|(([X0])\\|.*\\4\\|.*\\4\\|)|(\\|.+([X0]).+\\|.+\\6.+\\|.+\\6.+)\\|.*");
	std::smatch match2;

	if (std::regex_search(game_state, match2, re2) && match2.size() > 1){
		return 0;
	}


	// diagonals
	std::regex re3(".*\\|([X0]).*\\|.+\\1.+\\|.+.+\\1\\||\\|.*([X0])\\|.+\\2.+\\|\\2.+.+\\|.*");
	std::smatch match3;

	if (std::regex_search(game_state, match3, re3) && match3.size() > 1){
		return 0;
	}

	return 1;
}

/** Class used to keep a specific game state and all next possible game states */
class XO_tree {

	public:
		XO_tree(std::string initial_state) {
			state = initial_state;
		};

		/** State of the game for THIS iteration */
		std::string state = "";

		/** All next game states */
		XO_tree *next_tree[10] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

		/** Auxiliary index, used to know how many "next_tree's" we have so far */
		int next_tree_current_index = 0;

		/**
		 *  From the state member, return the index
		 * of the next free position(marked as '-'),
		 * or -1 if there are no more free positions
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

/**
 * Keep track of the current player
 *
 * 1 -> x, 0 -> 0
 */
int current_player = 1;

/** Program progress */
int ct = 0;

/** Generate the graph. Recursively calls itself until no other move is possible */
int generate_game_tree(XO_tree *tree, std::string piece){

	if(check_win_condition(tree->state) == 0) {
		return 0;
	}

	// Change player
	piece = (piece == "X") ? "0" : "X";

	// Get next free spot
	int index = tree->get_next_free_pos(0);

	while(index != 0){

		// Create a new node
		XO_tree *node = new XO_tree(tree->state);

		// Mark the free spot found previously with X or 0
		node->state.replace(index, 1, piece);

		// Store this new game state
		tree->next_tree[tree->next_tree_current_index] = node;

		// Recursively call the function for the new found state
		generate_game_tree(tree->next_tree[tree->next_tree_current_index++], piece);

		// Repeat for the next free spot, or stop if there are no free spots left
		index = tree->get_next_free_pos(index + 1);

	}

	// Display progress
	std::cout << ct++ << std::endl;

	return 0;
}

std::ofstream myfile;

/** Print nodes on a specific level */
void printLevel(XO_tree tree, int desired, int current){

	if(desired == current){

		if(tree.next_tree_current_index > 0){
			myfile << "\n" << tree.state << " -> ";

			for(int i = 0; i < tree.next_tree_current_index; i++) {
				myfile << tree.next_tree[i]->state << " ";
			}
		}
	} else {
		for(int i = 0; i < tree.next_tree_current_index; i++) {
			printLevel(*tree.next_tree[i], desired, current + 1);
		}
	}
}

/** Print all nodes, by level */
void print(XO_tree tree){

	for(int i = 0; i < 9; i++) {
		myfile << "\n Level: " << i << "\n";
		printLevel(tree, i, 0);
	}
}

int main() {

	// Initial state
	XO_tree xo_game_tree("|---|---|---|");

	// Open output file
	myfile.open ("results.txt");

	// Generate tree
	generate_game_tree(&xo_game_tree, "0");

	// Display tree
	print(xo_game_tree);

	// Close output file
	myfile.close();

	return 0;
}

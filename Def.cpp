#include "utility.hpp"

int main(int argc, char** argv) {
	
	int index = 0;
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < cols; j++){
			cells[i][j].setIndex(index);
			index++;
		}
	}
	int elems = rows * cols;
	
	MazeGenerator(elems);
	
	DrawMaze();
	
	cout << "\n";
	stack<pair<int, int> > stack = Depth_First_Search(rows-1, cols-1);
	cout << "Found Path with DFS:\n";
	while(!stack.empty()){
		pair<int, int> coord = stack.top();
		cout << "(" << coord.first << ", " << coord.second <<  ")" << " <- ";
		stack.pop();
	}
}

#include <iostream>
#include <cstdlib>
#include <ctime> 
#include <stack>
#include <map>

using namespace std;

// DEFINE THE NUMBER OF ROWS AND COLUMNS
const int rows = 50;
const int cols = 60;

class DisjointSet {
    private:
        int *rank;
		int *p;
		int n;

    public: 
        DisjointSet(int n) {
            rank = new int[n];
            p = new int [n];
            this->n = n;
			makeSet();
        }

        void makeSet() {
            for (int i = 0; i < n; i++) {
                p[i] = i;
                rank[i] = 0;
                
            }
        }

        int findSet(int x) {
            if ( x != p[x]) {
                p[x] = findSet(p[x]);
            }
            return p[x];
        }

        void Union(int x, int y) {
            link(findSet(x), findSet(y));
        }		

        void link(int x, int y) {

            // x and y already belong to the same set
            if (x == y)
                return;

            // if x has rank bigger than y, it becomes the parent
            if (rank[x] > rank[y]) {
                p[y] = x;
            }
            else {
                p[x] = y;

                //if ranks are equal, increment the rank of the parent (y)
                if (rank[x] == rank[y]){
                    rank[y] = rank[y] + 1;
                }
            }
        }
};

class Cell {
	private:
		int index;
    	bool up_wall;
    	bool right_wall;
    	
    public:
    	Cell() {
    		up_wall = true;
    		right_wall = true;
		}
		
		void setIndex (int ind){
			index = ind;
		}
		
		int getIndex(){
			return index;
		}
		
		void brakeWall (int wall) { // 0 = up_wall, 1 = right_wall
			if (wall == 0)
				up_wall = false;
			else 
				right_wall = false;
		}
		
		bool checkWall(int wall) {
			if (wall == 0)
				return up_wall;
			else 
				return right_wall;
		}
};

map<pair<int, int>, bool> visited;
	
Cell cells[rows][cols];

pair<bool, pair<int, int> > isMovePossible (pair<int, int> curr, int dir) {
	pair<bool, pair<int, int> > nestPair;
	bool isPossible;
	pair<int, int> nextMove;
	pair<int, int> indUP = make_pair(curr.first-1, curr.second);
	pair<int, int> indDOWN = make_pair(curr.first+1, curr.second);
	pair<int, int> indLEFT = make_pair(curr.first, curr.second-1);
	pair<int, int> indRIGHT = make_pair(curr.first, curr.second+1); 
	switch (dir) {
		case 1: //UP
			if (curr.first == 0){ // First row
				isPossible = false;
			}else if (!cells[curr.first][curr.second].checkWall(0) && !visited[indUP]){
				isPossible = true;
				nextMove = {curr.first-1, curr.second};
			}
			break;
		case 2: //RIGHT
			if (curr.second == cols){
				isPossible = false;
			} else if (!cells[curr.first][curr.second].checkWall(1) && !visited[indRIGHT]) {
				isPossible = true;
				nextMove = {curr.first, curr.second+1};
			}
			break;
		case 3: //DOWN
			if (curr.first == rows){
				isPossible = false;
			} else if (!cells[curr.first+1][curr.second].checkWall(0) && !visited[indDOWN]){
				isPossible = true;
				nextMove = {curr.first+1, curr.second};
			}
			break; //LEFT
		case 4:
			if (curr.second == 0) {
				isPossible = false;
			} else if (!cells[curr.first][curr.second-1].checkWall(1) && !visited[indLEFT]){
				isPossible = true;
				nextMove = {curr.first, curr.second-1};
			}
			break;
	}
	nestPair.first = isPossible;
	nestPair.second = nextMove;
	
	return nestPair;
}

stack<pair<int, int> > Depth_First_Search(int xG, int yG){
   	pair<int, int> startIndex = make_pair(0,0);
   	pair<int, int> currentIndex = startIndex;
   	pair<int, int> goalIndex = make_pair(xG, yG);
   	
   	stack<pair<int, int> > stackInd;
   	pair<bool, pair<int, int> > nestPair;
   	int directions[4] = {1, 2, 3, 4}; // 1 = UP; 2 = RIGHT; 3 = DOWN; 4 = LEFT
    	
    stackInd.push(startIndex);
    bool checkedAll;
   	visited[startIndex] = true;
    while (currentIndex != goalIndex){
    	checkedAll = false;
    	for (int i = 0; i < sizeof(directions); i++){
    		int direction = directions[i];
    		nestPair = isMovePossible(currentIndex, direction);
    		if (nestPair.first) { //move is possible
    			currentIndex = nestPair.second;
    			visited[currentIndex] = true;
    			stackInd.push(currentIndex);
    			break;
			}
			if (i==sizeof(directions)-1) checkedAll = true;
		}
		if (checkedAll) {
			stackInd.pop();
			currentIndex = stackInd.top();
		}
	}
	return stackInd;
}
	
void MazeGenerator(int elems) {
	int startIndex = 0;
	int goalIndex = elems-1;
	DisjointSet Maze(elems);
	srand(time(NULL));
	
	while(Maze.findSet(startIndex) != Maze.findSet(goalIndex)) {
		int rndRow = (rand() % rows);
		int rndCol = (rand() % cols);
		int rndWall = (rand() % 2);
		int index = cells[rndRow][rndCol].getIndex();
		int neighRightIndex = cells[rndRow][rndCol+1].getIndex();
		int neighUpIndex = cells[rndRow-1][rndCol].getIndex();
		
		if (rndWall == 0) {
			if (rndRow != 0) { //check if it's not the first row (UP wall is a border)
				if(cells[rndRow][rndCol].checkWall(0)) { //check if the wall exists
					if(Maze.findSet(index) != Maze.findSet(neighUpIndex)){ //check if the cell and its neighbour aren't in the same set
						Maze.Union(index, neighUpIndex);
						cells[rndRow][rndCol].brakeWall(0);
					}
				}
			}
		}
		else {
			if (rndCol != (cols - 1)) { //check if it's not the last column (RIGHT wall is a border)
				if (cells[rndRow][rndCol].checkWall(1)) {
					if(Maze.findSet(index) != Maze.findSet(neighRightIndex)){ //check if the cell and its neighbour aren't in the same set
						Maze.Union(index, neighRightIndex);
						cells[rndRow][rndCol].brakeWall(1);
					}
				}
			}
		}
	}
}

void DrawMaze(){
	for(int i = -1; i < rows; i++){
       for(int j = 0; j < cols; j++){
       		if (i==-1) { // "_" for the first row
	       		cout << " " << "_";
			}else if (i != -1){
				if (j == 0){
					if (i==0) cout << " ";
					else cout << "|";
					if (i < rows-1 && cells[i+1][j].checkWall(0)) // "_" only if the cell below has an up_wall and it's not the last row
						cout << "_";
					else if (i < rows-1 && !cells[i+1][j].checkWall(0)) 
						cout << " ";
					else if (i == rows-1) cout << "_"; // "_" for the last row
				}else{
					if (cells[i][j-1].checkWall(1)) // "|" only if the left cell has a right_wall
						cout << "|";
					else cout << " ";
					if (i == rows-1 || cells[i+1][j].checkWall(0)) // "_" only if the cell below has an up_wall
						cout << "_";
					else cout << " ";
				}
				if (j==cols-1) // "|" for the last column
					if (i != rows-1) cout << "|";
			}
		}
		cout << "\n";
    }
}

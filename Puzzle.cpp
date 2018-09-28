#include <iostream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <stack>
#include <vector>
#include <unordered_map>
#include <time.h>
using namespace std;

class Puzzle {
private:
#pragma region vars;
	int board[9];
	class Board {
	public:
		Board() {}
		Board* parent;
		int board[9];
		int cost;
		int depth;

	};
	class Compare{
	public:
		bool operator()(Board a, Board b){
			if (a.cost == b.cost)
				return a.depth > b.cost;
			return a.cost > b.cost;
		} 
	};
	priority_queue<Board, vector<Board>, Compare> pq;
	enum Move { left = -1, right = 1, up = -3, down = 3 };
	unordered_map <int, Board> visited;
	stack <Board> path;
	int maxDepth;
	int totalNodesCreated;
	bool runType;
#pragma endregion vars;

#pragma region Testers;
	struct Test {
		int noOfRuns = 0;
		int totalNodesCreated = 0;
		int totalTime = 0;
	};
	unordered_map<int, Test> testList;
	int largestRecord = 0;
#pragma endregion Testers;
	
	///----------------------HELPER FUNCTION--------------------///
	///---------------------------------------------------------///
#pragma region helperFunctions;
#pragma region noBoards;
	// gets the Manhatten distance of any single location inside of a board
	/* loc: index location, b= board to be searched, return= Manhatten distance for loc index*/
	int getMatDist(int loc, int b[9]) {
		return abs((loc % 3) - (b[loc] % 3)) + abs((int(loc / 3) - int(b[loc] / 3)));
	}

	int getMatDist(int loc) {
		return getMatDist(loc, board);
	}

	// checks if a board is "solveable"
	/* b= board to be checked, return= true if even number of inversions, otherwise false*/
	bool isLegal(int b[9]) {
		bool hasOnce[9] = { false };
		for each (int i in b) {
			if (i < 0 || i > 8 || hasOnce[i])
				return false;
			hasOnce[i] = true;
		}
		return getTotalInversion(b) % 2 == 0;
	}
	bool isLegal() {
		return isLegal(board);
	}

	//swapts two characters in a string
	/* s= string to be transformed, a/b= indexes of the string to be swaped*/
	void swap(string& s, int a, int b) {
		char temp = s[a];
		s[a] = s[b];
		s[b] = temp;
	}
	void swap(int ary[], int a, int b) {
		ary[a] += ary[b];
		ary[b] = ary[a] - ary[b];
		ary[a] = ary[a] - ary[b];
	}

	//this function returns the total amount of inversions for the board
	/* b= board to be checked, return= total number of inversions */
	int getTotalInversion(int b[9]) {
		int inv = 0;
		for (int i = 0; i < 9; i++) {
			if (b[i] == 0)
				continue;
			for (int j = i + 1; j < 9; j++)
				if (b[i] > b[j] && b[j] != 0)
					inv++;
		}
		return inv;
	}
	int getTotalInversion() {
		return getTotalInversion(board);
	}

	//makes a copy of a board
	/* original= board to be copied, copy= the copy */
	void makeCopy(int original[9], int copy[9]) {
		for (int i = 0; i < 9; i++)
			copy[i] = original[i];
	}

	//gets the sum of ALL Manhatten distances for every node
	/* b=board to be checked, return= total sum all manhatten distances */
	int getTotSumOfAllMatDist(int b[9]) {
		int sum = 0;
		for (int i = 0; i < 9; i++) {
			if (b[i] == 0)
				continue;
			sum += getMatDist(i, b);
		}
		return sum;
	}
	int getTotSumOfAllMatDist() {
		return getTotSumOfAllMatDist(board);
	}

	//checks if a "Move" is allowed from a current position
	/* m=move we are checking, zeroPosition=blank index, return=true if the move is allowed otherwise its false */
	bool isValidMove(Move m, int zeroPosition) {
		switch (m) {
		case left:
			return zeroPosition % 3 != 0;
			break;
		case right:
			return zeroPosition % 3 != 2;
			break;
		case up:
			return zeroPosition / 3 != 0;
			break;
		case down:
			return zeroPosition / 3 != 2;
			break;
		default:
			return false;
			break;
		}
	}
	bool isValidMove(Move m, int b[9]) {
		return isValidMove(m, findZero(b));
	}

	//find the 0 value and returns the index
	/* b= board to be searched, return=0's index*/
	int findZero(int b[9]) {
		for (int i = 0; i < 9; i++)
			if (b[i] == 0)
				return i;
		return -1;
	}

#pragma endregion noBoards;

	int getMisplaced(Board b) {
		int miss = 0;
		for (int i = 0; i < 9; i++)
			if (b.board[i] != i)
				miss++;
		return miss;
	}

	//inserts a "Board" object into the PQ and attaches the "parent" to this object
	/* copy=board to be copied, move= position the blank will be moved to, blank= 0's index */
	void insert(Board parent, int move, int blank) {
		totalNodesCreated++;

		Board child;
		child.parent = & visited.at(makeBoardIntoKey(parent));

		for (int i = 0; i < 9; i++)
			child.board[i] = parent.board[i];
		swap(child.board, blank, move + blank);

		child.depth = parent.depth+1;

		if (runType)
			child.cost = child.depth + getMisplaced(child);
			
			//getTotalInversion(child.board);
		else
			child.cost = child.depth + getTotSumOfAllMatDist(child.board);		

		if (visited.count(makeBoardIntoKey(child)) == 0) {
			pq.push(child);
		}
	}

	//inserts all valid options into the PQ
	/*  */
	void insertAllValid(Board b) {
		int zero = findZero(b.board);

		if (isValidMove(left, b.board)) {
			insert(b, left, zero);
		}
		if (isValidMove(right, b.board)) {
			insert(b, right, zero);
		}
		if (isValidMove(up, b.board)) {
			insert(b, up, zero);
		}
		if (isValidMove(down, b.board)) {
			insert(b, down, zero);
		}
	}

	//checks if the goal has been reached
	/* return= true if goal has been reached, otherwise false*/
	bool isAtGoal(Board b){
		return isAtGoal(b.board);
	}
	bool isAtGoal(int b[9]) {
		int ary[] = { 0,1,2,3,4,5,6,7,8 };
		for (int i = 0; i < 9; i++)
			if (b[i] != ary[i])
				return false;
		return true;
	}

	//transforms the board into a 9 digit number to use as a key
	/* b=board to be xformed */
	int makeBoardIntoKey(Board b) {
		int keyValue = 0;
		int scale = 1;
		for (int i = 8; i > -1; i--) {
			keyValue += (scale * b.board[i]);
			scale *= 10;
		}
		return keyValue;
	}


	//prints path
	void printPath(int startTime, bool pPath) {
		int endTime = clock();

		if (pPath) {
			while (!path.empty()) {
				Board steppingStone = path.top(); path.pop();

				for (int i = 0; i < 9; i++) {
					if (i % 3 == 0)
						cout << endl;
					cout << steppingStone.board[i] << " ";
				}
				cout << "\n----------------------h(n): "<< steppingStone.cost - steppingStone.depth<< " Cost: "<<steppingStone.cost
					<<" Depth: "<<steppingStone.depth;
			}

			cout << "\nDepth: " << maxDepth << endl
				<< "Total Nodes Generated: " << totalNodesCreated << endl
				<< "Run Time: " << endTime - startTime << endl;
		}

		if (largestRecord < maxDepth)
			largestRecord = maxDepth;

		if (testList.count(maxDepth) == 0) {
			Test temp;
			temp.noOfRuns = 1;
			temp.totalNodesCreated = totalNodesCreated;
			temp.totalTime = endTime - startTime;
			testList.emplace(maxDepth, temp);
		}
		else {
			Test *temp = &testList.at(maxDepth);
			temp->noOfRuns++;
			temp->totalNodesCreated += totalNodesCreated;
			temp->totalTime += (endTime - startTime);
		}
	}

	//checks if a string has any non number values
	bool isAllDigit(string s) {
		for each (char c in s)
			if (!isdigit(c))
				return false;
		return true;
	}
#pragma endregion helperFunctions;
	///---------------------------------------------------------///

	///-------------------Start Up Function----------------------///
	///----------------------------------------------------------///
#pragma region startUpFunction;
	//takes a string of numbers and spaces and makes it into an "board
	/* s=string of numbers, return=does not return anything however will input data into "board" */
	bool makeBoard(string s) {
		totalNodesCreated = 0;
		while (s.find(" ") != string::npos) {
			int val = s.find(" ");
			s.replace(val, 1, "");
		}
		bool passable= true;
		if (s.size() != 9 || !isAllDigit(s))
			passable = false;
		int count = 0;
		for each(char c in s) {
			board[count] = c - '0';
			count++;
		}
		return passable;
	}
	
	//creates a random string from 0-8
	/* return=outputs randomized number string*/
	string makeStringBoard() {
		string b = "012345678";
		srand(time(0));
		for (int i = 0; i < 9; i++)
			swap(b, i % 9, rand() % 9);
		return b;
	}

	//makes the inital board
	/*  */
	void initBoardCreation() {
		Board init;
		for (int i = 0; i < 9; i++)
			init.board[i] = board[i];
		init.depth = 0;
		init.parent = NULL;
		init.cost = getTotalInversion();		//

		pq.push(init);
	}
#pragma endregion startUpFunction;

public:
	Puzzle() {
		do {
			makeBoard(makeStringBoard());
		} while (!isLegal());
	}
	Puzzle(string list) {
		do {
			makeBoard(list);
			if (!isLegal() ) {
				cout << "Not a Legal Entry\nEnter a valid Entry\n";
				cin >> list;
			}
		} while (!isLegal());
	}
	void clear() {
		visited.clear();
		while (!path.empty())
			path.pop();
		while (!pq.empty())
			pq.pop();
	}
	void clearRecords() {
		largestRecord = 0;
		testList.clear();
	}
	void printRecords() {
		cout << largestRecord << endl;

		for (int i = 0; i < largestRecord + 1; i++) {
			if (testList.count(i) == 1) {
				Test t = testList.at(i);
				cout << "Depth: " << i << "\tAverage Cost: " << t.totalNodesCreated / t.noOfRuns
					<<"\tTotal Test Cases: " <<t.noOfRuns<<"\tAvg Run Time: "<<t.totalTime/t.noOfRuns<<endl;
			}
		}
	}

	void startGame(bool r) {
		runType = r;
		do {
			makeBoard(makeStringBoard());
		} while (!isLegal());
	}
	void startGame(string list, bool r) {
		runType = r;
		do {
			makeBoard(list);
			if (!isLegal()) {
				cout << "Not a Legal Entry\nEnter a valid Entry\n";
				cin >> list;
			}
		} while (!isLegal());
	}
	void run() {
		run(true);
	}
	void run(bool pPath) {
		int runTimeStart = clock();
		initBoardCreation();
		Board worker;

		do {			
			worker = pq.top();pq.pop();
			if (isAtGoal(worker))
				break;

			visited.emplace(makeBoardIntoKey(worker), worker);				
			insertAllValid(worker);

		} while (true);



		///////////////////////////////
		path.push(worker);
		maxDepth = worker.depth;

		Board *pathFinder = &worker;
		do {
			pathFinder = pathFinder->parent;
			if (pathFinder != NULL)
				path.push(*pathFinder);
		} while (pathFinder != NULL);
		printPath(runTimeStart, pPath);
	}
};
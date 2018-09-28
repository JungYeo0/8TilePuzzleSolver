#include <iostream>
#include <fstream>
#include "Puzzle.cpp"
using namespace std;


void main() {
	string s;
	ifstream inFile;
	char input, runChoice, numRuns;
	Puzzle p;
	string user;
	bool runType = true;
	int runs = 1;

	do {
		cout << "Select Input option\n"
			<< "1. Input your own puzzle\n"
			<< "2. Random Inputs\n"
			<< "e. Exit\n"
			<< ": ";
		cin >> input;

		do {
			cout << "Select Run type\n"
				<< "1. Using Inversions\n"
				<< "2. Using Manhatten Distances\n"
				<< ": ";
			cin >> runChoice;
			if (runChoice != '1' && runChoice != '2')
				cout << "Please Select Correct input\n";
		} while (runChoice != '1' && runChoice != '2');
		if (runChoice == '1')
			runType = true;
		else
			runType = false;


		switch (input) {
		case '1': {
			cout << "Input 8-tile puzzle: ";
			cin >> user;

			p.startGame(user, runType);
			p.run();
			break;
		}
		case '2': {
			cout << "Input Number of Runs: ";
			cin >> runs;

			for (int i = 0; i < runs; i++) {
				cout << "Run: " << i + 1 << endl;
				p.startGame(runType);
				p.run();
				p.clear();
			}
			break;
		}
		case 'e':
			break;
		case'p':
			p.printRecords();
			break;
		case 'a': {
			cout << "input number of tests: ";
			int testNum;
			cin >> testNum;
			cout << "Begining h1 Tests...\n";
			for (int i = 0; i < testNum; i++) {
				p.startGame(true);
				p.run(false);
				p.clear();
			}
			cout << "H1 Results\n";

			p.printRecords();
			p.clearRecords();

			cout << "Begining h2 Tests...\n";
			for (int i = 0; i < testNum; i++) {\
				p.startGame(false);
				p.run(false);
				p.clear();
			}
			cout << "H2 Results\n";
			p.printRecords();
			p.clearRecords();
			break;
		}
		case 'b': {
			if (inFile) {
				inFile.open("manualDepth.txt");
				cout << "Begining h1 Tests...\n";
				while (getline(inFile, s)) {
					if (isdigit(s[0])) {
						p.startGame(s, true);
						p.run(false);
						p.clear();
					}
				}
				cout << "H1 Results\n";
				p.printRecords();
				p.clearRecords();
				inFile.close();
				///------------------------------------
				inFile.open("manualDepth.txt");
				cout << "Begining h2 Tests...\n";
				while (getline(inFile, s)) {
					if (isdigit(s[0])) {
						p.startGame(s, false);
						p.run(false);
						p.clear();
					}
				}
				cout << "H1 Results\n";
				p.printRecords();
				p.clearRecords();
				inFile.close();
			}
			else {
				cout << "File not found\n";
			}

			break;
		}
		default:
			cout << "Incorrect input\n";
			break;
		}
		p.clear();

	} while (tolower(input) != 'e');

	p.clearRecords();
	system("PAUSE");
}

#include <queue>
#include <vector>
#include <memory>
#include "Play.h"
#include "Player.h"
using namespace std;

//Enumeration for return type of main function
enum {
	SUCCESS = 0,
	FAIL_WRONG_ARGUMENTS = 1,
	FAIL_FILE_OPEN = 2
};

int main(int argc, char* argv[]) {

	if (argc < 2) {
		//Check for the command line usage
		cerr << "ERROR:  usage: " << argv[0] << "<configuration_file_name>" << endl;
		return FAIL_WRONG_ARGUMENTS;
	}

	string playName;
	string currentLine;
	queue<SingleLine> information;
	ifstream config(argv[1]);

	if (!config.is_open()) {
		cerr << "ERROR:  Open file fail: " << argv[1] << endl;
		return FAIL_FILE_OPEN;
	}

	int characterCount = 0;
	bool nameFound = false;

	while (getline(config, currentLine)) {
		if (currentLine.size()>0) {
			if (!nameFound) {
				playName = currentLine;
				nameFound = true;
			}
			else {
				//Transfer the every single line into istringstream
				istringstream iss(currentLine);
				string character, path;
				iss >> character >> path;
				SingleLine temp(character, path);
				information.push(temp);
				characterCount++;
			}
		}
	}

	Play play(playName);
	//Use the unique_ptr to preserve the Player object
	vector<unique_ptr<Player>> players;
	
	for (int i = 1; i <= characterCount; i++) {
		ifstream tempInFile(information.front().second);
		if (!tempInFile.is_open()) {
			cerr << "ERROR:  Open file fail: " << information.front().second << endl;
			return FAIL_FILE_OPEN;
		}
		//Use new to pass a right value's address to a unique_ptr
		unique_ptr<Player> p(new Player(play, information.front().first, tempInFile));
		//Here use the move to transfer the move to vector
		players.push_back(move(p));
		players.back()->enter();
		information.pop();
	}

	auto iter = players.begin();
	while (iter != players.end()) {
		//players' iterator stores a pointer to Player object
		(*iter)->exit();
		iter++;
	}
	return SUCCESS;
}
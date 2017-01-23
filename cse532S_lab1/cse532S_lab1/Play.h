#ifndef PLAY_H
#define PLAY_H

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>
#include <iostream>
#include <regex>
#include <mutex>
#include <condition_variable>

using namespace std;

typedef pair<string, string> SingleLine;
typedef map<int,pair<string,string>> Line;
typedef Line::iterator LineIter;

class Play
{
public:
	Play():counter(1), firstLine(true) {}
	Play(string name) : playName(name), counter(1), firstLine(true) {}
	void recite(LineIter& iter);
private:
	int counter;
	//The boolean value used to determine whether it's firstLine right now
	bool firstLine;

	mutex m;
	condition_variable cv;
	string currentCharacter;
	string playName;
};

#endif PLAY_H


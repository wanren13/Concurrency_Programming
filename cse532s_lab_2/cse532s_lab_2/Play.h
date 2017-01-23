#ifndef PLAY_H
#define PLAY_H

#include <mutex>
#include <condition_variable>
#include <exception>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <thread>
#include <regex>
#include <memory>
#include <future>

#include "CodeException.h"

using namespace std;

//ACT
typedef future<bool> endFuture;

//sceneTitle data type
typedef vector<string> sceneTitles;
typedef sceneTitles::iterator sceneCountIter;

//lines datastructure 
typedef map<int, pair<string, string>> Line;
typedef Line::iterator LineIter;

//Enumeration for return type of main function
enum {
	SUCCESS,
	FAIL_WRONG_ARGUMENTS,
	FAIL_FILE_OPEN,
	BAD_ALLOCATION
};

class Play
{
public:
	Play(sceneTitles& ref,endFuture&& fut) :sceneRef(ref), on_stage(0), waiting(0), line_counter(1), scene_fragment_counter(1), firstLine(true), emergencyStop(false) {
		iter = sceneRef.begin();
		if (!ref.empty()) {
			cout << *iter << endl;
			iter++;
		}
		finishFlag = move(fut);
	}
	void recite(LineIter& iter, int sceneCount);
	void enter(int sceneCount);
	void exit();
	void checkAvailable(int max);
	bool checkPlayFinished();
	void emergentStop();
private:
	int line_counter;
	int scene_fragment_counter;
	int on_stage;
	int waiting;
	//The boolean value used to determine whether it's firstLine right now
	bool firstLine;
	string currentCharacter;

	mutex m;
	mutex m_enter;
	mutex m_check;
	condition_variable cv;
	condition_variable cv_enter;
	condition_variable cv_cue;
	sceneTitles& sceneRef;
	sceneCountIter iter;
	endFuture finishFlag;

	//reset current character and first line variables
	void resetCCandFL();

	//emergency stop
	bool emergencyStop;
};

#endif PLAY_H


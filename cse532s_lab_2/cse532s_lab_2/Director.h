#ifndef H_DIRECTOR
#define H_DIRECTOR

#include "Play.h"
#include "Player.h"

//ACT
typedef promise<bool> endPromise;

//play and player datatype
typedef shared_ptr<Play> playPtr;
typedef shared_ptr<Player> playerPtr;
typedef vector<shared_ptr<Player>> playerContainer;

//data structure to store the script information
typedef map<string, string> script;
typedef map<int, script> scriptsContainer;
typedef scriptsContainer::iterator scriptsIter;
typedef script::iterator sIter;

//exception
typedef shared_future<bool> result;

class Director
{
public:
	Director(const string& scriptName, unsigned int playerCount = 0, bool overRide = false);
	void cue();
	result getResult();
private:
	int maximum;
	bool isOverride;

	endPromise finished;

	sceneTitles titles;
	scriptsContainer scripts;
	playPtr play;
	playerContainer players;

	void emergencyStop();

	result sharedFut;
};

#endif
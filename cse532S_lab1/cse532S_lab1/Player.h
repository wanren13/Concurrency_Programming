#ifndef PLAYER_H
#define PLAYER_H

#include "Play.h"

using namespace std;

class Player
{
public:
	Player() : infile(ifstream()), play(Play()) {}
	Player(Play& p, const string& n, ifstream& in) 
		:play(p), name(n) {
		//Attention: using move to transfer the ownship of ifstream into this
		this->infile = move(in);
	}
	void read();
	void act();
	void enter();
	void exit();
private:
	Line lines;
	string name;
	ifstream infile;
	//Use the reference of play to ensure that the PLay remains only one instrance
	Play& play;
	thread t;
};

#endif


#include "Player.h"

void Player::read(string name, string script)
{
	ifstream infile(script);
	if (!infile.is_open()) {
		string error = "[ERROR]:  Open file fail: " + script;
		throw CodeException(FAIL_FILE_OPEN, error.c_str());
	}
	string currentLine;
	//Using regular expression to check the data for each line
	regex re("^\\s*(\\d+)\\s*([^\\d\\s].*?)\\s*$");
	while (getline(infile, currentLine)) {
		smatch sm;
		regex_match(currentLine, sm, re);
		if (sm.size() > 0) {
			//Once the line match the RE, come in this if condition and insert data
			lines[stoi(sm[1])] = SingleLine(name, sm[2]);
		}
	}
}

void Player::act()
{
	while (isActive) {
		if (!retrieving) {
			// acting
			if (!lines.empty()) {
				if (iter != lines.end()) {
					play.recite(iter, sceneCount);
					iter++;
				}
				else {
					exit();
				}
			}
			// no job anymore
			if (lines.empty() && play.checkPlayFinished()) {
				deactive();
			}
		}
		// else is waiting
	}
}

void Player::enter(int sceneCounter, string name, string script)
{
	play.enter(sceneCounter);
	busy = true;
	retrieving = true;
	sceneCount = sceneCounter;
	try {
		read(name, script);
	}
	catch (CodeException& e) {
		throw e;
	}
	iter = lines.begin();
	retrieving = false;
}

void Player::exit()
{
	lines.clear();
	busy = false;
	play.exit();
}

bool Player::isbusy()
{
	return busy;
}

void Player::activate()
{
	if (!isActive) {
		isActive = true;
		this->t = thread(&Player::act, this);
	}
}

void Player::deactive()
{
	if (isActive) {
		isActive = false;
	}
}

void Player::join()
{
	if (t.joinable()) {
		t.join();
	}
}

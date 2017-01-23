#include "Player.h"

void Player::read()
{
	string currentLine;
	//Using regular expression to check the data for each line
	while (getline(infile, currentLine)) {
		regex re("^\\s*(\\d+)\\s*([^\\d\\s].*?)\\s*$");
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
	LineIter iter = lines.begin();
	while (iter != lines.end()) {
		play.recite(iter);
		iter++;
	}
}

void Player::enter()
{
	this->t = thread([this] {
		this->read(); 
		this->act();
	});
}

void Player::exit()
{
	if (this->t.joinable()) {
		t.join();
	}
}

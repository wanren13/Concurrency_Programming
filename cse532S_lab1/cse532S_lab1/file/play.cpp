#include "play.h"

using namespace std;

void play::inner_print(ostream & os)
{
	context::iterator it = content.begin();
	string currentName = it->second.first;
	os << currentName << "." << endl;
	while (it != content.end()) {
		if (it->second.first != currentName) {
			currentName = it->second.first;
			os << endl << (*it).second.first << "." << endl;
		}
		os << it->second.second << endl;
		it++;
	}
}

play::play(string s)
{
	playName = s;
}

context & play::operator<<(const pair<int, pair<string, string>>& l)
{
	lock_guard<mutex> guard(play::lock);
	content[l.first] = l.second;
	return content;
}

void play::print(ostream & os)
{
	lock_guard<mutex> guard(play::lock);
	inner_print(os);
}

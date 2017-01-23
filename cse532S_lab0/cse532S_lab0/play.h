#ifndef FILE_H
#define FILE_H

#include <map>
#include <string>
#include <mutex>

using namespace std;

//( index, (character name, line) )
typedef map<int, pair<string, string>> context;;
typedef pair<int, pair<string, string>> line;
typedef pair<string, string> part;

class play {
private:
	string playName;
	context content;
	static mutex lock;
	void inner_print(ostream& os);
public:
	play(string s);
	context& operator << (const pair<int, pair<string, string>>& l);
	void print(ostream& os);
};


#endif

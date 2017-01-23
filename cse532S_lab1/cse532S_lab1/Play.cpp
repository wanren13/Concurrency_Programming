#include "Play.h"

void Play::recite(LineIter & iter)
{
	unique_lock<mutex> lock(m);
	cv.wait(lock, [&] {
		if (iter->first == counter) {
			return true;
		}
		else if (iter->first > counter) {
			return false;
		}
		else {
			//if the counter larger than index, should increment iterator
			cerr << "ERROR:  Counter larger than structured line's number" << endl;
			iter++;
			cv.notify_all();
			return true;
		}
	});
	if (firstLine) {
		currentCharacter = iter->second.first;
		cout << iter->second.first << "." << endl;
		firstLine = false;
	}
	else {
		if (currentCharacter != iter->second.first) {
			cout << endl << iter->second.first << "." << endl;
			currentCharacter = iter->second.first;
		}
	}
	
	cout << iter->second.second << endl;
	
	counter++;
	//The notify_all can be done after unlock
	lock.unlock();
	cv.notify_all();
}


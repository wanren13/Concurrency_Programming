#include "Play.h"

void Play::recite(LineIter & iter, int sceneCount)
{
	unique_lock<mutex> lock(m);
	bool largelineCounter = false;
	cv.wait(lock, [&] {
		if (emergencyStop) {
			return true;
		}

		if (sceneCount == scene_fragment_counter && iter->first == line_counter) {
			return true;
		}
		else if (scene_fragment_counter < sceneCount || (sceneCount == scene_fragment_counter && iter->first > line_counter)) {
			return false;
		}
		else {
			//to be finished
			cerr << "[ERROR]:  scene already passed or line already passed" << endl;
			cv.notify_all();
			largelineCounter = true;
			return true;
		}
	});
	if (!emergencyStop && !largelineCounter) {
		// emergency stop will notify this method to continue, and jump out out loop
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

		line_counter++;
		//The notify_all can be done after unlock
		lock.unlock();
		cv.notify_all();
	}
}

void Play::enter(int sceneCount)
{
	if (sceneCount < scene_fragment_counter) {
		throw exception("[ERROR]: scene counter less than current counter");
	}
	else if (sceneCount == scene_fragment_counter) {
		on_stage++;
	}
	else {
		//different condition variable and different mutex
		unique_lock<mutex> lock(m_enter);
		// some one is not ont the stage but keep waiting, so not available
		waiting++;
		cv_enter.wait(lock, [&] {
			return (sceneCount == scene_fragment_counter);
		});
		waiting--;
		on_stage++;
	}
}

void Play::exit()
{
	if (on_stage > 1) {
		on_stage--;
		cv_cue.notify_all();
	}
	else if (on_stage < 1) {
		throw exception("[ERROR]: lower bound of on stage exceed");
	}
	else {
		on_stage--;
		// if someone exit the stage, he can be cued, so notify
		cv_cue.notify_all();
		resetCCandFL();
		scene_fragment_counter++;
		line_counter = 1;
		if (iter != sceneRef.end()) {
			if (!iter->empty()) {
				cout << endl << *iter << endl;
			}
			iter++;
		}
		cv_enter.notify_all();
	}
}

void Play::checkAvailable(int max)
// check whether there is any available player
{
	unique_lock<mutex> lock(m_check);
	cv_cue.wait(lock, [&] {
		return (on_stage + waiting) < max;
	});
}

bool Play::checkPlayFinished()
// ACT cookie check, whether the scripts are all dispatched
{
	return finishFlag._Is_ready();
}

void Play::emergentStop()
{
	emergencyStop = true;
	// in case of some threads stuck with recite function, need to notify them all.
	cv.notify_all();
}

void Play::resetCCandFL()
// reset the firstLine and currentCharacter
{
	firstLine = true;
	currentCharacter = string();
}



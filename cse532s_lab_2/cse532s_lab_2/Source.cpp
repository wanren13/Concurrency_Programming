#include <sstream>
#include "Director.h"

int main(int argc, char* argv[]) {

	if (argc < 2 || argc > 5) {
		//Check for the command line usage
		cerr << "[ERROR]:  usage: " << argv[0] << "<script_file_name> <optional: minimum_player> <optional: -override>" << endl;
		return FAIL_WRONG_ARGUMENTS;
	}

	bool minimumProvided = false;
	unsigned int minimum = 0;
	if (argc >= 3) {
		istringstream in(argv[2]);
		// check for whether the third argument is integer or not
		if (!(in >> minimum && in.eof())) {
			cerr << "[ERROR]:  <optional: minimum_player> :'" << argv[2] << "' must be a integer!" << endl;
			return FAIL_WRONG_ARGUMENTS;
		}
		minimumProvided = true;
	}

	bool isoverride = false;
	if (argc == 4) {
		istringstream in(argv[3]);
		string override;
		in >> override;
		if (override != "-override") {
			cerr << "[ERROR]:  <optional: -override> :" << argv[3] << " must be '-override'!" << endl;
			return FAIL_WRONG_ARGUMENTS;
		}
		isoverride = true;
	}

	try {
		if (minimumProvided) {
			Director dic(argv[1], minimum,isoverride);
			dic.cue();
			shared_future<bool> fut = dic.getResult();
			// use future to capture exception between different threads
			try {
				fut.get();
			}
			catch (CodeException& e) {
				cerr << e.what() << endl;
				return e.errCode();
			}
		}
		else {
			Director dic(argv[1]);
			dic.cue();
			shared_future<bool> fut = dic.getResult();
			// use future to capture exception between different threads
			try {
				fut.get();
			}
			catch (CodeException& e) {
				cerr << e.what() << endl;
				return e.errCode();
			}
		}
		
	}
	catch (CodeException& error) {
		cerr << error.what() << endl;
		return error.errCode();
	}
	
	return SUCCESS;
}
#ifndef H_COEXCPT
#define H_COEXCPT

#include <exception>

using namespace std;

class CodeException : public exception {
public:
	CodeException() {}
	CodeException(int err_, const char* msg) : err(err_), exception(msg) {}
	CodeException(int err_, exception& e) : err(err_), exception(e) {}
	int errCode() const { return err; }
private:
	// store the error code
	int err;
};

#endif

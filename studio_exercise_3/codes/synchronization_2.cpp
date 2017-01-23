#include <iostream>
#include <vector>
#include <iterator>
#include <numeric>
#include <sstream>

using namespace std;

class wanren {
private:
	vector<int> wulei;
public:
	wanren();
	wanren& operator<<( const int& i);
	int operator()();
	void print();
};

wanren& wanren::operator<<(const int& i) {
	wulei.push_back(i);
	return *this;
}

int wanren::operator()() {
	return accumulate(wulei.begin(), wulei.end(), 0);
}
wanren::wanren() {
	wulei = vector<int>();
}

void wanren::print() {
	vector<int>::iterator it = wulei.begin();
	while (it != wulei.end()) {
		cout << *it<<endl;
		it++;
	}
}

int main(int argc, char * argv[]) {
	wanren test;
	for (int i = 1; i < argc; i++) {
		istringstream ss(argv[i]);
		int value;
		if (ss >> value) {
			test << value;
		}
	}
	cout << test() << endl;
	return 0;
}

#include <iostream>
#include <vector>
#include <iterator>
#include <numeric>
#include <sstream>
#include <thread>

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

void insertNumbers(wanren& w, const int num, const int seqNum) {
	for (int i = 0; i < 5; i++) {
		w << num + i;
		cout << "Insert - Thread: " << seqNum << " input: " << num + i << endl;
	}
}

void print(wanren& w,const int seqNum) {
	int sum = w();
	cout << "Thread num: " << seqNum << " has sum: " << sum << endl;
}


int main(int argc, char * argv[]) {
	wanren test;
	thread t[5];
	thread p[5];
	for (int count = 0; count < 5; count++) {
		t[count] = thread(insertNumbers, ref(test), count * 5, count + 1);
		p[count] = thread(print,ref(test), count+1);
		t[count].join();
		p[count].join();
	}

	return 0;
}

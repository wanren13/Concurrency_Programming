#include <iostream>
#include <vector>
#include <iterator>
#include <numeric>
#include <sstream>
#include <thread>
#include <mutex>

using namespace std;

class wanren {
private:
	vector<int> wulei;
	mutex dingding;
public:
	wanren();
	wanren& operator<<( const int& i);
	int operator()();
	void print();
};

wanren& wanren::operator<<(const int& i) {
	lock_guard<mutex> guard(dingding);
	wulei.push_back(i);
	return *this;
}

int wanren::operator()() {
	lock_guard<mutex> guard(dingding);
	return accumulate(wulei.begin(), wulei.end(), 0);
}
wanren::wanren() {
	wulei = vector<int>();
}

void wanren::print() {
	lock_guard<mutex> guard(dingding);
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
	}
	for (int count = 0; count < 5; count++) {
		t[count].join();
		p[count].join();
	}

	return 0;
}

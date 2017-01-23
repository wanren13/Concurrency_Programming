#include <iostream>
#include <vector>
#include <iterator>
#include <numeric>
#include <sstream>
#include <thread>
#include <mutex>

using namespace std;

template <class T>
class wanren {
private:
	vector<int> wulei;
	T dingding;
public:
	wanren();
	wanren& operator<<( const int& i);
	int operator()();
	void print();
	int factorial_of_sum();
};

template <class T>
int wanren<T>::factorial_of_sum() {
	lock_guard<T> guard(dingding);
	int sum = (*this)();
	int factorial = 1;
	for (int i = 1; i <= sum; i++) {
		factorial *= i;
	}
	return factorial;
}

template <class T>
wanren<T>& wanren<T>::operator<<(const int& i) {
	lock_guard<recursive_mutex> guard(dingding);
	wulei.push_back(i);
	return *this;
}

template <class T>
int wanren<T>::operator()() {
	lock_guard<recursive_mutex> guard(dingding);
	return accumulate(wulei.begin(), wulei.end(), 0);
}

template <class T>
wanren<T>::wanren() {
	wulei = vector<int>();
}

template <class T>
void wanren<T>::print() {
	lock_guard<recursive_mutex> guard(dingding);
	vector<int>::iterator it = wulei.begin();
	while (it != wulei.end()) {
		cout << *it<<endl;
		it++;
	}
}

template <class T>
void insertNumbers(wanren<T>& w, const int num, const int seqNum) {
	for (int i = 0; i < 5; i++) {
		w << num + i;
		cout << "Insert - Thread: " << seqNum << " input: " << num + i << endl;
	}
}

template <class T>
void print(wanren<T>& w,const int seqNum) {
	int sum = w.factorial_of_sum();
	cout << "Thread num: " << seqNum << " has factorial of sum: " << sum << endl;
}


int main(int argc, char * argv[]) {
	wanren<recursive_mutex> test;
	thread t[5];
	thread p[5];
	for (int count = 0; count < 5; count++) {
		t[count] = thread(insertNumbers<recursive_mutex>, ref(test), count * 5, count + 1);
		p[count] = thread(print<recursive_mutex>,ref(test), count+1);
	}
	for (int count = 0; count < 5; count++) {
		t[count].join();
	}
	for (int count = 0; count < 5; count++) {
		p[count].join();
	}

	return 0;
}

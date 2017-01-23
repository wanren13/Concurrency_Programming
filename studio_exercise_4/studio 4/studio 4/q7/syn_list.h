#ifndef H_SYNL
#define H_SYNL

#include <iostream>
#include <list>
#include <mutex>
#include <thread>

using namespace std;

template <typename T>
class Syn_list{
public:
	Syn_list();//{ slist = list<T>(); }
	Syn_list(int lwm, int hwm);
	void push_back(const T& val);
	void push_front(const T& val);
	T& pop_back();
	T& pop_front();
	bool empty();
	thread insertAndLaunch(int index,int val);
	thread popAndReturn(int index);

	bool loopBreaker;

	mutex m;
	condition_variable cvPop;
	condition_variable cvPush;
private:
	size_t lowWaterMark;
	size_t highWaterMark;
	list<T> slist;
};

template <typename T>
Syn_list<T>::Syn_list(){
	slist = list<T>();
	loopBreaker = false;
	lowWaterMark = 0;
	highWaterMark = 0;
}

template<typename T>
inline Syn_list<T>::Syn_list(int lwm, int hwm)
{
	slist = list<T>();
	loopBreaker = false;
	if (lwm >= 0) {
		lowWaterMark = lwm;
	}
	else {
		cout << "Invalid parameters for low water mark. So initialized with 0." << endl;
		lowWaterMark = 0;
	}
	if (hwm >= 0) {
		highWaterMark = hwm;
	}
	else {
		cout << "Invalid parameters for high water mark. So initialized with 0." << endl;
		highWaterMark = 0;
	}
}

template <typename T>
bool Syn_list<T>::empty(){
	lock_guard<mutex> lk(m);
	return slist.empty();
}

template <typename T>
void Syn_list<T>::push_back(const T& val){
	if (highWaterMark == 0) {
		lock_guard<mutex> lk(m);
		slist.push_back(val);
	}
	else {
		unique_lock<mutex> lk(m);
		cvPush.wait_for(lk, chrono::seconds(2), [this] {return slist.size() < highWaterMark; });
		slist.push_back(val);
		lk.unlock();
	}
}

template <typename T>
void Syn_list<T>::push_front(const T& val){
	if (highWaterMark == 0) {
		lock_guard<mutex> lk(m);
		slist.push_front(val);
	}
	else {
		unique_lock<mutex> lk(m);
		cvPush.wait_for(lk, chrono::seconds(2), [this] {return slist.size() < highWaterMark; });
		slist.push_front(val);
		lk.unlock();
	}
}

template <typename T>
T& Syn_list<T>::pop_back(){
	cout << "*************************" << slist.size() << "*************************" << endl;
	unique_lock<mutex> lk(m);
	int count = 0;
	do {
		if (lowWaterMark == 0) {
			cvPop.wait_until(lk, chrono::high_resolution_clock::now() + chrono::seconds(1));
		}
		else {
			cvPop.wait_until(lk, chrono::high_resolution_clock::now() + chrono::seconds(1), [this] {return (slist.size() >= lowWaterMark); });
		}
		if (!slist.empty()) {
			T temp = slist.back();
			slist.pop_back();
			lk.unlock();
			return temp;
		}
		else {
			count++;
			if (count > 5) {
				loopBreaker = true;
				return count;
			}
		}
	} while (slist.empty());
}

template <typename T>
T& Syn_list<T>::pop_front(){
	unique_lock<mutex> lk(m);
	if (lowWaterMark == 0) {
		cvPop.wait(lk, [this] {return !slist.empty(); });
	}
	else {
		cvPop.wait(lk, [this] {return (!slist.empty()) && (slist.size() >= lowWaterMark); });
	}
	T temp = slist.front();
	slist.pop_front();
	lk.unlock();
	return temp;
}

template <typename T>
thread Syn_list<T>::insertAndLaunch(int index,int val){
	return thread([=] {
		for (int i = 0; i < 20; i++) {
			this->push_front(i + val);
			cout << "push thread: " << index << " with value: " << i + val << endl;
		}
	});
}

template<typename T>
inline thread Syn_list<T>::popAndReturn(int index)
{
	return thread([=] {
		while (true) {
			auto temp = this->pop_back();
			if (loopBreaker) {
				break;
			}
			cout << "###pop### thread: " << index << " with value: " << temp << endl;
		}
	});
}


#endif
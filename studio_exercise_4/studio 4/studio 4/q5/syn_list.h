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
	void push_back(const T& val);
	void push_front(const T& val);
	T& pop_back();
	T& pop_front();
	bool empty();
	thread insertAndLaunch(int index,int val);
	thread popAndReturn(int index);

	static mutex m;
	condition_variable cv;
private:
	list<T> slist;
};

template <typename T>
Syn_list<T>::Syn_list(){
	slist = list<T>();
}

template <typename T>
bool Syn_list<T>::empty(){
	lock_guard<mutex> lk(Syn_list<T>::m);
	return slist.empty();
}

template <typename T>
void Syn_list<T>::push_back(const T& val){
	lock_guard<mutex> lk(Syn_list<T>::m);
	slist.push_back(val);
	Syn_list<T>::cv.notify_one();
}

template <typename T>
void Syn_list<T>::push_front(const T& val){
	lock_guard<mutex> lk(Syn_list<T>::m);
	slist.push_front(val);
	Syn_list<T>::cv.notify_one();
}

template <typename T>
T& Syn_list<T>::pop_back(){
	unique_lock<mutex> lk(Syn_list<T>::m);
	Syn_list<T>::cv.wait(lk, [this] {return !slist.empty(); });
	T temp = slist.back();
	slist.pop_back();
	lk.unlock();
	return temp;
}

template <typename T>
T& Syn_list<T>::pop_front(){
	unique_lock<mutex> lk(Syn_list<T>::m);
	Syn_list<T>::cv.wait(lk, !slist.empty());
	T temp = slist.front();
	slist.pop_front();
	lk.unlock();
	return temp;
}

template <typename T>
thread Syn_list<T>::insertAndLaunch(int index,int val){
	return thread([=] {
		for (int i = 0; i < 100; i++) {
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
			cout << "pop thread: " << index << " with value: " << temp << endl;
		}
	});
}


#endif
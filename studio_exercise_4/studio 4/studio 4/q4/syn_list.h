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
	Syn_list();// { slist = list<T>(); }
	void push_back(const T& val);
	void push_front(const T& val);
	T& pop_back();
	T& pop_front();
	bool empty();
	thread insertAndLaunch(int index,int val);
	static mutex m;
private:
	list<T> slist;
};

template <typename T>
Syn_list<T>::Syn_list(){
	slist = list<T>();
}

template <typename T>
bool Syn_list<T>::empty(){
	return slist.empty();
}

template <typename T>
void Syn_list<T>::push_back(const T& val){
	slist.push_back(val);
}

template <typename T>
void Syn_list<T>::push_front(const T& val){
	lock_guard<mutex> lk(Syn_list<T>::m);
	slist.push_front(val);
}

template <typename T>
T& Syn_list<T>::pop_back(){
	lock_guard<mutex> lk(Syn_list<T>::m);
	T temp = slist.back();
	slist.pop_back();
	return temp;
}

template <typename T>
T& Syn_list<T>::pop_front(){
	T temp = slist.front();
	slist.pop_front();
	return temp;
}

template <typename T>
thread Syn_list<T>::insertAndLaunch(int index,int val){
	return thread([=]{ 
		for (int i = 0; i < 100; i++){
			this->push_front(i+val);
		}
		while (!this->empty()){
			auto tmp = this->pop_back();
			cout << "pop thread: " << index << " with value: " << tmp << endl;
		}
	});
}


#endif
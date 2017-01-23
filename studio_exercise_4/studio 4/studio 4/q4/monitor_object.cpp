#include "syn_list.h"
#include <string>
using namespace std;

template <typename T>
mutex Syn_list<T>::m;

int main(int argc, char* argv[]){
	Syn_list<int> s;
	thread t1 = s.insertAndLaunch(1,0);
	thread t2 = s.insertAndLaunch(2,100);
	thread t3 = s.insertAndLaunch(3,200);
	t1.join();
	t2.join();
	t3.join();

	system("pause");
	return 0;
}
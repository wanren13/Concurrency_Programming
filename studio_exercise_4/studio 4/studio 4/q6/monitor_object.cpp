#include "syn_list.h"
#include <string>
#include <queue>
using namespace std;

int main(int argc, char* argv[]){
	Syn_list<int> s(10,15);
	thread t1 = s.insertAndLaunch(1,0);
	thread t2 = s.insertAndLaunch(2, 100);
	thread t3 = s.insertAndLaunch(3, 200);
	thread p1 = s.popAndReturn(1);
	thread p2 = s.popAndReturn(2);
	thread p3 = s.popAndReturn(3);
	t1.join();
	t2.join();
	t3.join();
	p1.join();
	p2.join();
	p3.join();

	system("pause");
	return 0;
}
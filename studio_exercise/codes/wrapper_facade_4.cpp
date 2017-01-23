#include <iostream>
#include <thread>

using namespace std;

void hello() {
	cout << endl << "hello world" << endl;
	cout.flush();
}

int main(int,char * []) {
	thread t(hello);
	t.join();
	
	system("pause");
	return 0;
}
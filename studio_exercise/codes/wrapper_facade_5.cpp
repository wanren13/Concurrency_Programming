#include <iostream>
#include <thread>

using namespace std;

void hello() {
	cout << endl << "hello world" << endl;
	cout.flush();
}

int main(int,char * []) {
	thread t1(hello);
	thread t2(hello);
	thread t3(hello);
	thread t4(hello);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	system("pause");
	return 0;
}
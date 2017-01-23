#include <iostream>

using namespace std;

void function() {
	cout << endl << "hello world" << endl;
	cout.flush();
}

int main(int,char * []) {
	function();
	system("pause");
	return 0;
}
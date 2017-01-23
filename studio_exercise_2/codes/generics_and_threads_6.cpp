#include <iostream>
#include <numeric>
#include <thread>
#include <fstream>
#include <string>

using namespace std;

void sum(string s) {
	ifstream file(s);

	int myArray[7] = {};
	for (int i = 0; i < 7; i++) {
		file >> myArray[i];
	}

	int *begin = myArray;
	int *end = begin + sizeof(myArray) / sizeof(*myArray);
	int init = 0;

	cout<< accumulate(begin, end, init)<<endl;
}

int main(int, char *[]) {
	
	string s1 = "array1.txt";
	string s2 = "array2.txt";
	string s3 = "array3.txt";

	thread t1(sum, s1);
	thread t2(sum, s2);
	thread t3(sum, s3);

	t1.join();
	t2.join();
	t3.join();


	system("pause");
	return 0;
}
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

int main(int, char *[]) {
	int arr[] = { 1, 2, 3, 4, -5, 6 ,-7 ,8};
	
	cout << sizeof(arr) << endl;
	cout << sizeof(*arr) << endl;
	int max = sizeof(arr) / sizeof(*arr);
	int *ptr = arr + max ;
	cout << *ptr << endl;

	ostream_iterator<int> output(cout, " ");
	copy(arr, ptr, output);

	system("pause");
	return 0;
}
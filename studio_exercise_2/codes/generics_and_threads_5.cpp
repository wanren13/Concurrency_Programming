#include <iostream>
#include <numeric>
#include <thread>

using namespace std;

void sum(int* begin, int* end, int init) {
	cout<< accumulate(begin, end, init)<<endl;
}

int main(int, char *[]) {
	int arr[5][7] = { {1,2,3,4,-1,28,9},{12,34,95,-1,6,7,8},{-1,-45,-98,-4,12,14,0},{9,5,45,32,-10,-100,1},{3,2,1,0,-1,-2,-3} };

	int height = sizeof(arr) / sizeof(*arr);
	int width = sizeof(*arr) / sizeof(**arr);

	/*int *ptr = *arr + width;
	cout << *ptr << endl;*/
	for (int i = 0; i < height; i++) {
		int init = 0;
		int *begin = *arr + width*i;
		int *end = *arr + width*(i + 1);
		thread t(sum, begin, end, init);
		t.join();
	}

	system("pause");
	return 0;
}
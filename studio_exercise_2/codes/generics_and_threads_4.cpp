#include <iostream>
#include <numeric>

using namespace std;

int main(int, char *[]) {
	int arr[5][7] = { {1,2,3,4,-1,28,9},{12,34,95,-1,6,7,8},{-1,-45,-98,-4,12,14,0},{9,5,45,32,-10,-100,1},{3,2,1,0,-1,-2,-3} };

	int height = sizeof(arr) / sizeof(*arr);
	int width = sizeof(*arr) / sizeof(**arr);

	/*int *ptr = *arr + width;
	cout << *ptr << endl;*/

	for (int i = 0; i < height; i++) {
		int init = 0;
		cout << accumulate(*arr+width*i,*arr+width*(i+1), init) << endl;
	}

	system("pause");
	return 0;
}
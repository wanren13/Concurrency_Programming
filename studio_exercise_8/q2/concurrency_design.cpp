#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <thread>

using namespace std;

int main(int argc, char* argv[]) {
	srand(time(NULL));

	int arr1[4][16];
	int arr2[16][4];
	int arr3[8][8];

	for (int i = 0; i < (sizeof(arr1) / sizeof(arr1[0])); i++) {
		for (int j = 0; j < (sizeof(arr1[0])/sizeof(arr1[0][0])); j++) {
			arr1[i][j] = rand() % 100;
			cout << "(" << i << "," << j << "): " << arr1[i][j] << " with address: " << arr1[i] + j << " or " << &(arr1[i][j]) << endl;
		}
		cout << "-------------------------------------" << endl;
	}

	cout << endl << "#####################################" << endl;

	for (int i = 0; i < (sizeof(arr2) / sizeof(arr2[0])); i++) {
		for (int j = 0; j < (sizeof(arr2[0]) / sizeof(arr2[0][0])); j++) {
			arr2[i][j] = rand() % 100;
			cout << "(" << i << "," << j << "): " << arr2[i][j] << " with address: " << arr2[i] + j << " or " << &(arr2[i][j]) << endl;
		}
		cout << "-------------------------------------" << endl;
	}

	cout << endl << "#####################################" << endl;

	for (int i = 0; i < (sizeof(arr3) / sizeof(arr3[0])); i++) {
		for (int j = 0; j < (sizeof(arr3[0]) / sizeof(arr3[0][0])); j++) {
			arr3[i][j] = rand() % 100;
			cout << "(" << i << "," << j << "): " << arr3[i][j] << " with address: " << arr3[i] + j << " or " << &(arr3[i][j]) << endl;
		}
		cout << "-------------------------------------" << endl;
	}



	cout << "hardware: " << thread::hardware_concurrency() << endl;
	

	return 0;
}
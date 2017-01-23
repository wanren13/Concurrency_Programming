#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <utility>
#include <time.h>

using namespace std;
typedef pair<int, int> pos;

mutex m;

// why I give the work to deal with a row to a thread instead of using a column
// because they are address closer, check the data proximity and false sharing
void addition(int arr1[4][16], int arr2[16][4], int arrA[4][4], pos leftUpper, pos rightLower) {
	for (int i = leftUpper.first; i <= rightLower.first; i++) {
		for (int j = leftUpper.second; j <= rightLower.second; j++) {
			arrA[i][j] = arr1[i][j] + arr2[i][j];
		}
	}
}

int main(int argc, char* argv[]) {
	srand(time(NULL));

	int arr1[4][16] = { {61,21,83,4,56,29,66,63,79,72,80,31,59,54,48,56} , 
						{32,93,11,36,87,40,44,25,80,17,88,10,17,16,72,4} , 
						{30,6,20,46,52,73,71,67,95,64,65,75,71,25,63,69} , 
						{18,41,89,31,94,75,76,11,17,69,27,0,53,67,95,8} };
	int arr2[16][4] = { {51,18,70,17},{83,80,52,45},{9,5,63,43},{57,34,29,6},
						{0,27,75,80},{60,42,41,51},{14,1,75,79},{91,80,30,72},
						{87,88,26,13},{87,60,6,34},{4,94,27,70},{53,56,42,75},
						{76,84,61,90},{51,76,54,18},{98,2,92,16},{11,7,70,97} };
	int arrA[4][4];

	for (int i = 0; i < (sizeof(arr1) / sizeof(arr1[0])); i++) {
		for (int j = 0; j < (sizeof(arr1[0])/sizeof(arr1[0][0])); j++) {
			cout << "(" << i << "," << j << "): " << arr1[i][j] << " with address: " << arr1[i] + j << " or " << &(arr1[i][j]) << endl;
		}
		cout << "-------------------------------------" << endl;
	}

	cout << endl << "#####################################" << endl;

	for (int i = 0; i < (sizeof(arr2) / sizeof(arr2[0])); i++) {
		for (int j = 0; j < (sizeof(arr2[0]) / sizeof(arr2[0][0])); j++) {
			cout << "(" << i << "," << j << "): " << arr2[i][j] << " with address: " << arr2[i] + j << " or " << &(arr2[i][j]) << endl;
		}
		cout << "-------------------------------------" << endl;
	}

	cout << endl << endl;

	cout << "hardware cores number: " << thread::hardware_concurrency() << endl;
	int concurrencyCount = thread::hardware_concurrency();

	vector<thread> vec;
	for (int i = 0; i < 4; i++) {
		
		vec.push_back(thread(addition, arr1, arr2, arrA, pos(i, 0), pos(i, 3)));
	}

	auto iter = vec.begin();
	while (iter != vec.end()) {
		iter->join();
		iter++;
	}

	for (int i = 0; i < (sizeof(arrA) / sizeof(arrA[0])); i++) {
		for (int j = 0; j < (sizeof(arrA[0]) / sizeof(arrA[0][0])); j++) {
			cout << "(" << i << "," << j << "): " << arrA[i][j] << " with address: " << arrA[i] + j << " or " << &(arrA[i][j]) << endl;
		}
		cout << "-------------------------------------" << endl;
	}

	return 0;
}
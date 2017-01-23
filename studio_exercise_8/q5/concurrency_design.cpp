#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;
typedef pair<int, int> pos;

mutex m;

void multiply(int arr1[4][16], int arr2[16][4], int arrR[4][4], pos leftUpper, pos rightLower) { 
	int length = sizeof(arr1[0]) / sizeof(arr1[0][0]);
	for (int i = leftUpper.first; i <= rightLower.first; i++) {
		for (int j = leftUpper.second; j <= rightLower.second; j++) {
			arrR[i][j] = 0;
			for (int k = 0; k < length; k++) {
				arrR[i][j] += arr1[i][k] * arr2[k][j];
			}
		}
	}

	lock_guard<mutex> lock(m);
	cout << leftUpper.first << "," << leftUpper.second << " to " << rightLower.first << "," << rightLower.second << endl;
}

int main(int argc, char* argv[]) {

	int arr1[4][16] = { {61,21,83,4,56,29,66,63,79,72,80,31,59,54,48,56} , 
						{32,93,11,36,87,40,44,25,80,17,88,10,17,16,72,4} , 
						{30,6,20,46,52,73,71,67,95,64,65,75,71,25,63,69} , 
						{18,41,89,31,94,75,76,11,17,69,27,0,53,67,95,8} };
	int arr2[16][4] = { {51,18,70,17},{83,80,52,45},{9,5,63,43},{57,34,29,6},
						{0,27,75,80},{60,42,41,51},{14,1,75,79},{91,80,30,72},
						{87,88,26,13},{87,60,6,34},{4,94,27,70},{53,56,42,75},
						{76,84,61,90},{51,76,54,18},{98,2,92,16},{11,7,70,97} };
	int arrR[4][4];

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
		int mod = i / 2;//0011
		int res = i % 2;//0101
		//output (0,0)(1,1) (0,2)(1,3) (2,0)(3,1) (2,2)(3,3)
		vec.push_back(thread(multiply, arr1, arr2, arrR, pos(2*mod, 2*res), pos(2*mod+1,2*res+1)));
	}

	auto iter = vec.begin();
	while (iter != vec.end()) {
		iter->join();
		iter++;
	}

	for (int i = 0; i < (sizeof(arrR) / sizeof(arrR[0])); i++) {
		for (int j = 0; j < (sizeof(arrR[0]) / sizeof(arrR[0][0])); j++) {
			cout << "(" << i << "," << j << "): " << arrR[i][j] << " with address: " << arrR[i] + j << " or " << &(arrR[i][j]) << endl;
		}
		cout << "-------------------------------------" << endl;
	}

	return 0;
}
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <utility>
#include <time.h>

using namespace std;
typedef pair<int, int> pos;

mutex m;

void search(int arr[8][8], int target, pos leftUpper, pos rightLower) {
	int count = 0;
	for (int i = leftUpper.first; i <= rightLower.first; i++) {
		for (int j = leftUpper.second; j <= rightLower.second; j++) {
			if (arr[i][j] == target) {
				count++;
			}
		}
	}
	lock_guard<mutex> lock(m);
	cout << "Target : " << target << " From :(" << leftUpper.first << "," << leftUpper.second << ") to (" 
		<< rightLower.first << "," << rightLower.second << ") is " << count << endl;
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
	int arr3[8][8] = { {70,18,47,91,19,7,37,13},{32,73,64,60,15,25,33,33},
					   {8,48,98,33,10,2,8,67},{76,28,68,77,9,87,57,90}, 
					   {29,10,47,34,46,6,56,74},{9,16,7,9,11,19,46,43}, 
					   {26,22,62,42,65,60,98,94},{42,42,76,89,61,49,3,39}, };

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

	cout << endl << "#####################################" << endl;

	for (int i = 0; i < (sizeof(arr3) / sizeof(arr3[0])); i++) {
		for (int j = 0; j < (sizeof(arr3[0]) / sizeof(arr3[0][0])); j++) {
			cout << "(" << i << "," << j << "): " << arr3[i][j] << " with address: " << arr3[i] + j << " or " << &(arr3[i][j]) << endl;
		}
		cout << "-------------------------------------" << endl;
	}



	cout << "hardware cores number: " << thread::hardware_concurrency() << endl;
	int concurrencyCount = thread::hardware_concurrency();

	vector<thread> vec;
	for (int i = 0; i < concurrencyCount; i++) {
		
		vec.push_back(thread(search, arr3, rand() % 100, pos(0, 0), pos(7, 7)));
	}

	auto iter = vec.begin();
	while (iter != vec.end()) {
		iter->join();
		iter++;
	}

	vec.clear();
	for (int i = 0; i < concurrencyCount; i++) {

		vec.push_back(thread(search, arr3, 15 , pos(i, 0), pos(i, 7)));
	}

	iter = vec.begin();
	while (iter != vec.end()) {
		iter->join();
		iter++;
	}

	return 0;
}
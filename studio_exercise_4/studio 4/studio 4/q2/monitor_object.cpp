#include "syn_list.h"
using namespace std;

int main(int argc, char* argv[]){
	Syn_list<int> b;
	b.push_back(1);
	b.push_back(2);
	b.push_back(3);

	int x = b.pop_front();
	int y = b.pop_front();

	cout << x << y << endl;

	system("pause");
	return 0;
}
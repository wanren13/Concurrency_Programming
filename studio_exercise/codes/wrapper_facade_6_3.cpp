#include <iostream>

using namespace std;

void hello(int argc,char * argv[]) {
	cout << endl << argv[1]<< endl;
	cout.flush();
}

int main(int argc,char *argv[]) {
	try
	{
		if (argc > 1){
			hello(argc, argv);
		}
		else{
			cout << "more input!" << endl;
		}
	}
	catch (exception &error) {
		cerr << "Exception: " << error.what() << endl;
	}

	system("pause");
	return 0;
}
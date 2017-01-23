#include <iostream>
#include <thread>

using namespace std;

void hello(int argc,char * argv[]) {
	cout << endl << argv[1]<< endl;
	cout.flush();
}

int main(int argc,char *argv[]) {
	try
	{
		if (argc > 1){
			thread t1(hello, argc, argv);
			thread t2(hello, argc, argv);
			thread t3(hello, argc, argv);
			thread t4(hello, argc, argv);
			t1.join();
			t2.join();
			t3.join();
			t4.join();
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
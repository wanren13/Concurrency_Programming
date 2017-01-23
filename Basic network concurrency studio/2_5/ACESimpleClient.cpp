#if defined(_WIN32) && !defined(WIN32)
#define WIN32
#endif

#include <iostream>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>

using namespace std;

enum{
	SUCCESS,
	NO_COMMAND_ERROR,
	CONNECTION_FAIL,
	STREAM_MESSAGE_SENDING_ERROR
};

int main(int argc, char *argv[]){
	if (argc <= 1){
		cout << "Plesae input command!" << endl;
		return NO_COMMAND_ERROR;
	}

	for (int i = 1; i < argc; i++){
		cout << argv[i] << endl;
	}

	ACE_INET_Addr address(1234, ACE_LOCALHOST);
	ACE_SOCK_CONNECTOR connector;
	ACE_SOCK_STREAM stream;


	if (connector.connect(stream, address) >= 0){
		for (int i = 1; i < argc; i++){
			string temp = string(argv[i]) + " ";
			if (stream.send_n(temp.c_str(),temp.length()) < 0){
				cout << "Stream message sending failed!" << endl;
				return STREAM_MESSAGE_SENDING_ERROR;
			}
		}
		stream.close();
	}
	else{
		cout << "Connector failed on connecting!" << endl;
		return CONNECTION_FAIL;
	}

	return SUCCESS;
}
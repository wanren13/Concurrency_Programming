#if defined(_WIN32) && !defined(WIN32)
#define WIN32
#endif

#include <iostream>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>

using namespace std;

enum{
	SUCCESS,
	TOO_MANY_COMMANDS_ERROR,
	SOCK_ACCEPTOR_OPEN_FAIL,
	SOCKET_CONNECTION_FAIL
};

int main(int argc, char *argv[]){
	if (argc > 1){
		cout << "Please do not use command!" << endl;
		return TOO_MANY_COMMANDS_ERROR;
	}
	cout << argv[0] << endl;

	ACE_INET_Addr address(1234, ACE_LOCALHOST);
	ACE_SOCK_ACCEPTOR acceptor;
	ACE_SOCK_STREAM stream;

	if (acceptor.open(address, 1) >= 0){
		ACE_TCHAR temp[30];
		size_t n = 30;
		address.addr_to_string(temp,n);
		cout << temp << endl;
	}
	else{
		cout << "Sock acceptor open fail" << endl;
		return SOCK_ACCEPTOR_OPEN_FAIL;
	}

	for (;;){
		if (acceptor.accept(stream) < 0){
			cout << "Socket Connection failed!" << endl;
			return SOCKET_CONNECTION_FAIL;
		}
		char buf[BUFSIZ];
		int n;
		while ((n = stream.recv(buf, BUFSIZ)) > 0){
			_write(1, buf, n);
		}

		cout << "Socket is closed!" << endl;
		stream.close();

	}

	return SUCCESS;
}
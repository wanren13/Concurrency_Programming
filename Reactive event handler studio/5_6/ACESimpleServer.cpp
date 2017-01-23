#if defined(_WIN32) && !defined(WIN32)
#define WIN32
#endif

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include <ace/INET_Addr.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/SOCK_Stream.h>
#include <ace/Event_Handler.h>
#include <ace/Reactor.h>
#include <ace/Time_Value.h>
#include <ace/Signal.h>

using namespace std;

struct message{
	int id;
	char buf[BUFSIZ];
};

enum{
	SUCCESS,
	TOO_MANY_COMMANDS_ERROR,
	SOCK_ACCEPTOR_OPEN_FAIL,
	SOCKET_CONNECTION_FAIL,
	SOCKET_RECEIVE_ERROR,
	SOCKET_SEND_ERROR
};

class ServerStream : public ACE_Event_Handler{
public:
	virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE){
		message msg;
		stream.recv_n(&msg, sizeof(message));

		if (msg.id != 0){
			cout << msg.buf << endl;

			string backInfo = "Message from server: Received!";
			message backmsg;
			strcpy_s(backmsg.buf, backInfo.c_str());
			stream.send_n(&backmsg, sizeof(message));
		}

		return SUCCESS;
	}

	ACE_SOCK_STREAM& getStream(){
		return stream;
	}

	ACE_HANDLE get_handle(void) const{
		return stream.get_handle();
	}

private:
	ACE_SOCK_STREAM stream;
};

class Server : public ACE_Event_Handler{
public:
	Server(){
		counter = 0;

		ACE_INET_Addr address(1234, ACE_LOCALHOST);
		if (acceptor.open(address, 1) >= 0){
			ACE_TCHAR temp[30];
			size_t n = 30;
			address.addr_to_string(temp, n);
			cout << temp << endl;
		}
		else{
			cout << "Sock acceptor open fail" << endl;
		}
		
	}

	virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE){
		unique_ptr<ServerStream> temp(new ServerStream());
		ACE_SOCK_STREAM& tempStream = temp->getStream();
		if (acceptor.accept(tempStream) < 0){
			cout << "Socket Connection failed!" << endl;
			return SOCKET_CONNECTION_FAIL;
		}

		message msg;
		tempStream.recv_n(&msg, sizeof(message));

		if (msg.id == 0){
			counter++;
			message idUpdate;
			idUpdate.id = counter;
			tempStream.send_n(&idUpdate, sizeof(message));
			ACE_Reactor::instance()->register_handler(&(*temp), ACE_Event_Handler::READ_MASK);
			streams.push_back(move(temp));
		}

		return SUCCESS;
	}

	virtual int handle_signal(int signum, siginfo_t * = 0, ucontext_t * = 0){
		cout << "SINGAL" << endl;
		return SUCCESS;
	}

	ACE_HANDLE get_handle(void) const{
		return acceptor.get_handle();
	}
private:
	int counter;

	vector<unique_ptr<ServerStream>> streams;
	ACE_SOCK_ACCEPTOR acceptor;
};

int main(int argc, char *argv[]){
	if (argc > 1){
		cout << "Please do not use command!" << endl;
		return TOO_MANY_COMMANDS_ERROR;
	}
	cout << argv[0] << endl;

	Server server;
	ACE_Reactor::instance()->register_handler(&server, ACE_Event_Handler::ACCEPT_MASK);

	ACE_Sig_Set sigset;
	sigset.sig_add(SIGINT);
	ACE_Reactor::instance()->register_handler(sigset, &server);
	ACE_Reactor::instance()->register_handler(&server, ACE_Event_Handler::SIGNAL_MASK);
	ACE_Reactor::instance()->run_reactor_event_loop();

	return SUCCESS;
}

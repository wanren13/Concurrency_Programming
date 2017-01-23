#if defined(_WIN32) && !defined(WIN32)
#define WIN32
#endif

#include <iostream>
#include <string>
#include <exception>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>
#include <ace/Event_Handler.h>
#include <ace/Time_Value.h>
#include <ace/OS.h>

#include <ace/Reactor.h>

using namespace std;

struct message{
	int id;
	char buf[BUFSIZ];
};

enum{
	SUCCESS,
	NO_COMMAND_ERROR,
	CONNECTION_FAIL,
	STREAM_MESSAGE_SENDING_ERROR
};

class Client : public ACE_Event_Handler{
public:
	Client(int argc, char* argv[]){
		id = 0;

		for (int i = 1; i < argc; i++){
			content += string(argv[i]);
			content += " ";
		}
		ACE_INET_Addr address("127.0.0.1:1234");
		ACE_SOCK_CONNECTOR connector;
		if (connector.connect(stream, address) < 0){
			cout << "Connector failed on connecting!" << endl;
		}
		else{
			cout << "connection success and content is :" << content << endl;
			ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::WRITE_MASK);
			ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK);
		}
	}

	virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE){
		message msg;
		stream.recv_n(&msg, sizeof(message));
		if (id == 0){
			id = msg.id;
			message forwardInfo;
			strcpy_s(forwardInfo.buf, content.c_str());
			stream.send_n(&forwardInfo, sizeof(message));
			cout << "id updated!" << endl;
		}
		else{
			cout << msg.buf << endl;
		}
		
		return SUCCESS;
	}
	virtual int handle_output(ACE_HANDLE h = ACE_INVALID_HANDLE){
		cout << h << endl;
		message msg;
		msg.id = 0;
		if (stream.send_n(&msg, sizeof(message))){
			cout << "Message : " << content << "   ---Send!" << endl;
			return SUCCESS;
		}
		else{
			cout << "Client output handler error!" << endl;
			return STREAM_MESSAGE_SENDING_ERROR;
		}
	}

	ACE_HANDLE get_handle(void) const{
		return stream.get_handle();
	}

private:
	int id;
	ACE_SOCK_STREAM stream;
	string content;
};

int main(int argc, char *argv[]){
	if (argc <= 1){
		cout << "Plesae input command!" << endl;
		return NO_COMMAND_ERROR;
	}

	cout << "Client Input:" << endl;
	for (int i = 1; i < argc; i++){
		cout << argv[i] << endl;
	}
	cout << "Finished." << endl;

	Client client(argc, argv);

	
	ACE_Reactor::instance()->run_reactor_event_loop();


	return SUCCESS;
}



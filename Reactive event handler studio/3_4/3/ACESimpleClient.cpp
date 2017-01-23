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

enum{
	SUCCESS,
	NO_COMMAND_ERROR,
	CONNECTION_FAIL,
	STREAM_MESSAGE_SENDING_ERROR
};

class Client : public ACE_Event_Handler{
public:
	Client(int argc, char* argv[]){
		for (int i = 1; i < argc; i++){
			content += string(argv[i]);
			content += " ";
		}
		ACE_INET_Addr address("127.0.0.1:1234");
		ACE_SOCK_CONNECTOR connector;
		if (connector.connect(stream, address) < 0){
			cout << "Connector failed on connecting!" << endl;
		}

		cout << "connection success and content is :" << content << endl;
	}

	virtual int handle_timeout(const ACE_Time_Value&, const void* ptr = nullptr){
		/*ACE_INET_Addr address("127.0.0.1:1234");
		ACE_SOCK_CONNECTOR connector;
		if (connector.connect(stream, address) < 0){
			cout << "Connector failed on connecting!" << endl;
			return CONNECTION_FAIL;
		}

		cout << "connection success and content is :" << content << endl;*/

		/*if (stream.send_n(content.c_str(), content.length()) < 0){
			cout << "Stream message sending failed!" << endl;
			return STREAM_MESSAGE_SENDING_ERROR;
		}*/

		return 0;
	}
	virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE){
		char buf[BUFSIZ];

		if (stream.recv_n(buf, BUFSIZ)){
			cout << buf << endl;
			return 0;
		}
		else{
			cout << "Client input handler error!" << endl;
			return -1;
		}
	}
	virtual int handle_output(ACE_HANDLE h = ACE_INVALID_HANDLE){
		if (stream.send_n(content.c_str(), content.length())){
			cout << "Message : " << content << "   ---Send!" << endl;
			return 0;
		}
		else{
			cout << "Client output handler error!" << endl;
			return -1;
		}
	}

	ACE_HANDLE get_handle(void) const{
		return stream.get_handle();
	}

private:
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

	ACE_Reactor::instance()->register_handler(&client, ACE_Event_Handler::ALL_EVENTS_MASK);
	ACE_Reactor::instance()->run_reactor_event_loop();

	return SUCCESS;
}

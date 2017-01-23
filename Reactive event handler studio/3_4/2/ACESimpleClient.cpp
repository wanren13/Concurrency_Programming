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
	}
	virtual int handle_timeout(const ACE_Time_Value&, const void* ptr = nullptr){
		ACE_INET_Addr address("127.0.0.1:1234");
		ACE_SOCK_CONNECTOR connector;
		if (connector.connect(stream, address) < 0){
			cout << "Connector failed on connecting!" << endl;
			throw(CONNECTION_FAIL);
		}

		cout << "connection success and content is :" << content << endl;

		if (stream.send_n(content.c_str(), content.length()) < 0){
			cout << "Stream message sending failed!" << endl;
			return STREAM_MESSAGE_SENDING_ERROR;
		}
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
	cout << "Finished.-" << endl;

	Client client(argc, argv);
	ACE_Time_Value currentTime = ACE_OS::gettimeofday();
	ACE_Time_Value interval(3, 0);
	ACE_Time_Value delay(2);

	ACE_Reactor::instance()->schedule_timer(&client, 0, delay, interval);
	ACE_Reactor::instance()->run_event_loop();

	return SUCCESS;
}
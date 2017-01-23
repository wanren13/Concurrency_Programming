#if defined(_WIN32) && !defined(WIN32)
#define WIN32
#endif

#include <iostream>
#include <string>
#include <exception>
#include <memory>

#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>
#include <ace/Event_Handler.h>
#include <ace/Time_Value.h>
#include <ace/OS.h>

#include <ace/Reactor.h>
#include <ace/Connector.h>

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

class SimpleServiceHandler : public ACE_Svc_Handler <ACE_SOCK_STREAM, ACE_NULL_SYNCH> {
public:
	void passContent(string c){
		content = c;
	}

	virtual int open(void* = 0){
		id = 0;

		cout << "Connection established!" << endl;
		message msg;
		msg.id = id;
		if (peer().send_n(&msg, sizeof(message)) == -1){
			cout << "Client output handler error!" << endl;
			return STREAM_MESSAGE_SENDING_ERROR;
		}

		ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK);

		return SUCCESS;
	}

	virtual int handle_input(ACE_HANDLE h = ACE_INVALID_HANDLE){
		message msg;
		peer().recv_n(&msg, sizeof(message));
		
		if (id == 0){
			id = msg.id;
			cout << "ID is updated!" << endl;

			ACE_Time_Value interval(3);
			ACE_Time_Value delay(0);

			ACE_Reactor::instance()->schedule_timer(this, 0, delay, interval);			
		}
		else{
			cout << msg.buf << endl;
		}

		return SUCCESS;
	}

	virtual int handle_timeout(const ACE_Time_Value& time, const void* = 0){
		message forwardInfo;
		strcpy_s(forwardInfo.buf, content.c_str());
		peer().send_n(&forwardInfo, sizeof(message));
		return SUCCESS;
	}

	~SimpleServiceHandler(){
		this->shutdown();
	}

private:
	int id;
	string content;
};

class SimpleConnector : public ACE_Connector<SimpleServiceHandler,ACE_SOCK_CONNECTOR>{
public:
	SimpleConnector(int argc, char* argv[]){
		ACE_INET_Addr address(1234, ACE_LOCALHOST);

		string content;
		for (int i = 1; i < argc; i++){
			content += string(argv[i]);
			content += " ";
		}

		SerPtr = new SimpleServiceHandler();
		SerPtr->passContent(content);

		if (this->connect(SerPtr, address) == -1){
			cout << "Connection failed!" << endl;
		}
	}

	~SimpleConnector(){
		delete SerPtr;
		this->close();
	}

private:
	SimpleServiceHandler* SerPtr;
};

int main(int argc, char *argv[]){
	if (argc <= 1){
		cout << "Plesae input command!" << endl;
		return NO_COMMAND_ERROR;
	}

	cout << "-------------->Client Input:" << endl;
	for (int i = 1; i < argc; i++){
		cout << argv[i] << endl;
	}
	cout << "-------------->Finished." << endl;


	//the conncetion part
	SimpleConnector s(argc, argv);

	

	ACE_Reactor::instance()->run_reactor_event_loop();


	return SUCCESS;
}



#include <iostream>
#include <atomic>

using namespace std;

template <typename T>
class lock_free_stack {
private:
	struct node {
		T data;
		node* next;
		node(T const& data_) :
			data(data_)
		{}
	};

	atomic<node*> head;
public:
	void push(T const& data) {
		node* const new_node = new node(data);
		new_node->next = head.load();
		//the cew check if the head equal to new_node->next, if it does, replace head with new_node
		//if it doesn't, replace the new_node->next with head.(because meanwhile the head has already been modified)
		while (!head.compare_exchange_weak(new_node->next, new_node));
	}
};

int main(int argc, char* argv[]) {
	
	lock_free_stack<int> ins;

	for (int i = 0; i < 10; i++) {
		ins.push(i);
	}

	return 0;
}
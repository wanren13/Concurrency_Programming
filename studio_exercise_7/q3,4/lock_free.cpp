#include <atomic>
#include <memory>
#include <iostream>
#include <vector>
#include <thread>

using namespace std;

template<typename T>
class lock_free_stack
{
private:
	struct node
	{
		std::shared_ptr<T> data;
		node* next;
		node(T const& data_) :
			data(std::make_shared<T>(data_))
		{}
	};
	std::atomic<node*> head;
public:
	void push(T const& data)
	{
		node* const new_node = new node(data);
		new_node->next = head.load();
		while (!head.compare_exchange_weak(new_node->next, new_node));
	}
	std::shared_ptr<T> pop()
	{
		node* old_head = head.load();
		while (old_head &&
			!head.compare_exchange_weak(old_head, old_head->next));
		return old_head ? old_head->data : std::shared_ptr<T>();
	}
};

template <typename T>
void dataFlow(lock_free_stack<T>& lfs, bool odd, int count,vector<T>& vec) {
	int start = odd ? 1 : 0;
	for (int i = 0; i < count; i++) {
		int number = start + i * 2;
		lfs.push(number);
	}

	for (int i = 0; i < count; i++) {
		vec.push_back(*(lfs.pop()));
	}

}

int main(int argc, char* argv[]) {
	
	lock_free_stack<int> ins;
	vector<int> vec1, vec2;
	int count = 100;


	thread t1 = thread(dataFlow<int>, ref(ins), true, count, ref(vec1));
	thread t2 = thread(dataFlow<int>, ref(ins), false, count, ref(vec2));

	t1.join();
	t2.join();

	for (int i = 0; i < count; i++) {
		cout << "vec 1: [" << i << "]:  " << vec1[i] << endl;
	}
	for (int i = 0; i < count; i++) {
		cout << "vec 2: [" << i << "]:  " << vec2[i] << endl;
	}
	return 0;
}
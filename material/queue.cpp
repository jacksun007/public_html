/*
 * queue.cpp
 *
 * Exercise 9, question 5
 *
 */

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <ctime>

template<typename T>
class Queue {
	struct Node {
		Node * next;
		T data;	
		Node(const T & data, Node * next=nullptr) 
			: next(next), data(data) {}	
	} * head, * tail;
	
public:
	Queue() : head(nullptr), tail(nullptr) {}
	
	~Queue() { 
		Node * curr = head;
		while (curr != nullptr) {
			Node * temp = curr;
			curr = curr->next;
			delete temp;
		}
	}
			
	T * front() {
		if (head == nullptr) {
			return nullptr;
		}
		return &head->data;
	}

    bool push_back(const T & data) {
		Node * node = new Node(data);
		
		if (node == nullptr) {
			return false;
		}
		
		if (head == nullptr) {
			head = tail = node;
		}
		else {
			tail->next = node;
			tail = node;
		}
		
		return true;
	}
	
	bool pop_front() {
		Node * node;

		if (head == nullptr) {
			return false;
		}
		
		node = head;
		head = head->next;
		if (head == nullptr) {
			tail = nullptr;
		}
		
		delete node;
		return true;
	}
};

using namespace std;

int main() 
{
    auto q = Queue<float>();
    
    ::srand(::time(NULL));
    
    for (int i = 0; i < 10; i++) {
        q.push_back((float)(rand() % 1000) / 100);
    }
    
    float * fp;
    while ((fp = q.front())) {
        cout << *fp << endl;
        auto ret = q.pop_front();
        assert(ret);
    }
    
    return 0;
}

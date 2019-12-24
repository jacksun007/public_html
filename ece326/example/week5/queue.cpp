#include <iostream>
#include <utility>

using namespace std;

class Queue {
	struct Node {
		Node * next;
		void * data;
		
		Node(void * data, Node * next=nullptr) 
			: next(next) , data(data) {}	
		~Node() { /* managed by Queue */ }
	} * head, * tail;
	void (* dest_f)(void *);
	
public:
	Queue(void (* destroy)(void *))
		: head(nullptr)
		, tail(nullptr)
		, dest_f(destroy)
	{}
	
	~Queue() { 
		Node * curr = head;
		while (curr != nullptr) {
			Node * temp = curr;
			curr = curr->next;
			dest_f(temp->data);
			delete temp;
		}
	}
		
	bool push_back(void * data) {
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
	
	void * front() {
		if (head == nullptr) {
			return nullptr;
		}
		return head->data;
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

template<typename T>
class Fifo : private Queue {
	static void destroy(void * ptr) {
		delete (T *)ptr;
	}

public:	
	Fifo() : Queue(&Fifo<T>::destroy) {}
	
	bool push_back(T && elem) {
		return Queue::push_back(new T(std::move(elem)));		
	}
	
	T * front() {
		return (T *)Queue::front();
	}
	
	bool pop_front() {
		return Queue::pop_front();
	}
};

struct Point
{
	Point(int x, int y) : x(x), y(y) {}
	Point(Point && rhs) : x(rhs.x), y(rhs.y) {}

	int x;
	int y;
	
	friend ostream & operator<<(ostream & os, const Point & p);
};

ostream & operator<<(ostream & os, const Point & p)
{
	os << "(" << p.x << ", " << p.y << ")";
	return os;
}

int main() {
	Fifo<Point> queue;
	
	cout << queue.front() << endl;
	
	queue.push_back(Point(1, 1));
	queue.push_back(Point(2, 2));
	queue.push_back(Point(3, 3));
	queue.push_back(Point(4, 4));
	
	cout << *(Point *)queue.front() << endl;
	queue.pop_front();
	cout << *(Point *)queue.front() << endl;
	
	return 0;
}



#include <cstddef>
#include <iostream>


template <typename T>
class Stack
{
public:
	bool empty() const
	{
		return top_ == nullptr;
	}
	
	void pop()
	{
		if (top_)
		{
			Node *node = top_;
			top_ = node->next;
			delete node;
			--size_;
		}
	}
	
	void push(T element)
	{
		Node *node = new Node(element);
		if (top_) node->next = top_;
		top_ = node;
		++size_;
	}
	
	std::size_t size() const
	{
		return size_;
	}
	
	T top() const
	{
		return top_->data;
	}
	
private:
	struct Node
	{
		Node(T data) : data(data) { }
		T     data;
		Node *next = nullptr;
	};
	
	Node*       top_  = nullptr;
	std::size_t size_ = 0;
};


// Special queue implemented using two stacks.
// The second stack is used to hold the last element
// of the queue, as well as a temporary store when
// pushing a new element to the queue.
template <typename T>
class MyQueue
{
public:
	T back() const
	{
		if (!s2_.empty())
		{
			return s2_.top();
		}
		else
		{
			return s1_.top();
		}
	}
	
	bool empty() const
	{
		return s1_.empty();
	}
	
	T front() const
	{
		return s1_.top();
	}

	void push(T element)
	{
		if (s1_.empty())
		{
			s1_.push(element);
		}
		else if (s2_.empty())
		{
			s2_.push(element);
		}
		else
		{
			T old = s2_.top();
			s2_.pop();
			
			do
			{
				s2_.push(s1_.top());
				s1_.pop();
			}
			while (!s1_.empty());
			
			s2_.push(old);
			
			do
			{
				s1_.push(s2_.top());
				s2_.pop();
			}
			while (!s2_.empty());
			
			s2_.push(element);
		}
	}
	
	void pop()
	{
		s1_.pop();
		
		if (s1_.empty() && !s2_.empty())
		{
			s1_.push(s2_.top());
			s2_.pop();
		}
	}
	
	std::size_t size() const
	{
		return s1_.size() + s2_.size();
	}

private:
	Stack<T> s1_, s2_;	
};


template <typename T>
std::ostream& operator<<(std::ostream& os, const MyQueue<T>& queue)
{
	os << "MyQueue(size=" << queue.size();
	
	if (!queue.empty())
	{
		os << ",front=" << queue.front() << ",back=" << queue.back();
	}
	
	os << ")";
	
	return os;
}


int main()
{
	MyQueue<int> q;
	
	std::cout << q << std::endl;
	q.push(1);
	std::cout << q << std::endl;
	
	q.push(2);
	std::cout << q << std::endl;
	
	q.push(3);
	std::cout << q << std::endl;
	
	q.pop();
	std::cout << q << std::endl;
	
	q.pop();
	std::cout << q << std::endl;
	
	q.pop();
	std::cout << q << std::endl;
}

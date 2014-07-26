#include <cstddef>
#include <functional>
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
	
	friend std::ostream& operator<<(std::ostream& os, const Stack<T>& stack)
	{
		os << "Stack(size=" << stack.size_;
		
		if (stack.top_)
		{
			os << ",elements=" << stack.top_->data;

			for (Node *node = stack.top_->next; node; node = node->next)
			{
				os << " " << node->data;
			}
		}
		
		os << ")";
		
		return os;
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

template <typename T, typename Compare = std::greater<T>>
void sort(Stack<T>& stack)
{
	Compare  compare;
	Stack<T> store;
	
	while (!stack.empty())
	{
		if (store.empty() ||
		    compare(store.top(), stack.top()))
		{
			store.push(stack.top());
			stack.pop();
		}
		else
		{
			T temp = stack.top();
			stack.pop();
			
			std::size_t n = 0;
			
			do
			{
				stack.push(store.top());
				store.pop();
				++n;
			} while (!store.empty() &&
				     !compare(store.top(), temp));
			
			store.push(temp);
			
			for (; n; --n)
			{
				store.push(stack.top());
				stack.pop();
			}
		}
	}
	
	while (!store.empty())
	{
		stack.push(store.top());
		store.pop();
	}
}

int main()
{
	Stack<int> stack;
	
	stack.push(1);
	stack.push(2);
	stack.push(7);
	stack.push(21);
	stack.push(5);
	
	std::cout << stack << std::endl;
	sort(stack);
	std::cout << stack << std::endl;
}

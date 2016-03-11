#include <iostream>
#include <memory>

using namespace std;

int* create_shared(int i) {
	auto ptr1 = make_shared<int>(i);            // prints garbage
	//auto ptr1 = shared_ptr<int>(new int(i));  // prints i --> 9
	int* j = ptr1.get();
	return j;
}

int main()
{
	auto j = create_shared(9);
	cout << *j << endl;
}
#include <iostream>
using namespace std;

// std::enable_if implementation for reference. If B evaluates to
// TRUE at compile time, it has the subtype T. If it evaluates to 
// FALSE, it is the base without subtype T.
//
// A list of compile time evaluation of traits for B can be found
// at https://en.cppreference.com/w/cpp/header/type_traits.
//
// template<bool B, class T = void>
// struct enable_if {};
 
// template<class T>
// struct enable_if<true, T> { typedef T type; };
//

// Template specialization on functions.
// The code is from https://eli.thegreenplace.net/2014/sfinae-and-enable_if/.
template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

template <class T, enable_if_t<is_integral<T>::value>* = nullptr>
void do_stuff(T t) {
		cout << "do_stuff integral\n";
		// an implementation for integral types (int, char, unsigned, etc.)
}

template <class T, enable_if_t<!is_integral<T>::value>* = nullptr>
void do_stuff(T t) {
		cout << "do_stuff not integral\n";
		// an implementation for class types
}

// Template specialization on classes.
// NOTE: you must declare an unspecific baseClass before specializing. 
template <typename T, typename base = void>
class baseClass;

template <class T> 
class baseClass<T, typename enable_if<is_class<T>::value>::type>{
 public:
	 int b;
	 void print() {
			cout << "baseClass has var b: " << b << endl;
	 }
};

template <class T> 
class baseClass<T, typename enable_if<!is_class<T>::value>::type>{
 public:
	 int a;
	 void print() {
			cout << "baseClass has var a: " << a << endl;
	 }
};

class myClass{};

int main() {
	 do_stuff<int>( 0 );
	 do_stuff<myClass>( myClass() );
	 
	 baseClass<int> c;
	 c.a = 5;
	 c.print();

	 baseClass<myClass> d;
	 d.b = 7;
	 d.print();
	 return 0;
}

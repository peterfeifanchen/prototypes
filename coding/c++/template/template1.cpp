#include<iostream>

// This is a basic template instantiation.

template< typename T >
class Foo {
   typedef T value_type;
   value_type v_;

 // if void callFoo() {}, the method below would be a redefinition error in the
 // compiler.
 public: 
   void callFoo();
};

template< typename T >
void Foo<T>::callFoo() {
   std::cout << "callFoo" << std::endl;
}

int main( int argc, char** argv ) {
   Foo<int> f;
   f.callFoo();
}

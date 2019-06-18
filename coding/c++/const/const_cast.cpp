#include<iostream>
#include<typeinfo>

class Foo {
   int val;

 public:

   // Const_cast can be used to cast away const on a function
   void FooIs( int v ) const {
      //val = v //Compiler error as FooIs is a const function
      ( const_cast< Foo * >( this ) )->val = v;
   }

   // Const_cast can be used to pass const var to a non-const function parameter
   void FooIs( int* v ) {
      val = *v;
   }

   // Undefined behavior to write to a const value even if you cast it away
   void FooV( int* v ) {
      *v = 15;
   }

   void Print() {
      std::cout << "Foo is " << val << std::endl;
   }
};

int main() {

   Foo x;
   x.FooIs( 4 );
   x.Print();

   const int v = 10;
   x.FooIs( const_cast<int*>(&v) );
   x.Print();

   x.FooV( const_cast<int*>(&v) );
   std::cout << "v is " << v << std::endl;

   // Volatile is casted away by const_cast
   int a1 = 40; 
   const volatile int* b1 = &a1;  
   // type is PVKi, pointer to volatile and constant integer
   std::cout << "typeid of b1 " << typeid(b1).name() << std::endl;

   int* c1 = const_cast <int *> (b1); 
   // type is Pi, pointer to integer
   std::cout << "typeid of c1 " << typeid(c1).name() << std::endl; 

   return 0;
}


#include <iostream>


// Partial specialization requires only that the specialization be more specific
// than its base. However it is an error if its not more specific than its base.

template< class C, class E >
class A {}; // base template

template< class C >
class A< C, C > {
 public:
   void Foo() {
      std::cout << "Partial specialization where both template params are the same" << std::endl;
   }
};

// Template specialization must be deducible. This means that the template parameter
// has to appear just by itself at least once.
template< int I, int E >
class B {};

template< int I >
class B< I, 2 > {

};

// Partial template specialization picks the most specific template. If two
// templates are equally specific, it is an error. The following would produce a
// compiler error.
template< class F, class E > 
class C {};

template< class F >
class C< F, int*> {};

template< class F >
class C< int*, F> {}; 

// Partial template specialization of members
template<class T, int I> 
class D {
 public:
   void f(); // member declaration
};
 
template<class T, int I>
void D<T,I>::f() {
   std::cout << "Base template member f()" << std::endl;
} // primary template member definition
 
template<class T>
class D<T,2> {
 public:
   void f();
   void g();         
   void h();            
};
         
template<class T>
void D<T, 2>::g() {
   std::cout << "D1 specialized member g()" << std::endl;
} // partial specialized member definition

template<>
void D<char, 2>::h() {
   std::cout << "D2 fully specialized member h()" << std::endl;
} // full specialization of partial specialization
                         
int main() {
   // Basic partial specialization 
   A<int, int> a0;
   a0.Foo(); // partial template

   // Undefined template specialization as two partials are equally likely
   //C<int*, int*> c0; //Compiler error
   C<char, int*> c1;

   // Partial template specialization of members
   D<char,0> d0;
   d0.f(); // primary template
   D<char,2> d1;
   d1.g(); // partial template
   d1.h(); // fully specialized template
   
   // d1.f(); // compiler error: no definition of f() in the partial
}

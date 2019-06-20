#include<iostream>
#include<vector>

// We show how to do full template specialization
// For full explanation, it is available here:
// https://en.cppreference.com/w/cpp/language/template_specialization
//
// The key points are:
//  1) Explicit specialization must be defined after the general one in the same
//     namespace and after all uses of the explicit specialization
//  2) Can be declared, but not defined and used just like a ptr
//  3) Template parameters in specialization may be omitted if they can be deduced
//  4) Template specialization may be nested
//  5) Template specialization may be on member functions


// Regular simple explicit template specialization
namespace basic {
   template< class T1 > 
   class A {
    public:
       template< typename U > 
       class C {};
       void Afoo() { std::cout << "General class Afoo()" << std::endl; }
   };

   // Declared here but defined outside of namespace
   template<> 
   class A<int>;

   template<>
   class A<char>;

   template<> 
   template<typename U> class A<bool>::C {
    public:
      void Cfoo() {
         std::cout << "Bool class member class C specialization" << std::endl;
      }
   };
}

template<> 
class basic::A<int> {
 public:
    void Afoo() { std::cout << "Int class Afoo() explicit specialization" << std::endl; }
};

// We can also template specialize functions
template< typename T >
void sort( std::vector<T> &a ) {
   std::cout << "General function sort()" << std::endl;
};

template<>
void sort<int>( std::vector<int> &a ) {
   std::cout << "Int function sort() explicit specialization" << std::endl;
}

// We show how to specialize members of a specialized class
template<>
class basic::A<char> {
 public:
    void charFoo();
};

void basic::A<char>::charFoo() {
   std::cout << "Char class member charFoo specialization" << std::endl;
}

// We show how nexted template class works
template< class T1 > class A {
   template< class T2 > class B {
      template<class T3> void mf1(T3); // member template
   };
};

template <> // specialization for A
template < class T2 > // unspecialized B
class A<int>::B {
 public:   
   template<class T3> void mf1( T3 val ) {
      std::cout << "Nested class templates can have child template unspecialized" << std::endl;
   }
}; 

template <> // specialization for A
template <> // specialization for B
template <class T3> void A<int>::B<int>::mf1( T3 val ) {
      std::cout << "Nested class templates specialization" << std::endl;
}

int main() {
   basic::A<int> a;
   a.Afoo();

   basic::A<double> b;
   b.Afoo();

   std::vector<double> c;
   sort<double>( c );

   std::vector<int> d;
   sort<int>( d );

   basic::A<char> e;
   e.charFoo();

   basic::A<bool>::C<int> f;
   f.Cfoo();

   A<int>::B<char> g;
   g.mf1<int>( 3 );

   A<int>::B<int> h;
   h.mf1<int>( 3 );
}

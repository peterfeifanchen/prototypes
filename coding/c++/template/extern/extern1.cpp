#include<iostream>
#include "header.h"

// A c++ template is instantiated multiple times if used with the same template
// arguments in different compilation units (e.g., .o). This could lead to code
// bloat that is unnecessary. These instantiations are bound as weak references
// that later are thrown away by the linker. This results in wasted compilation
// time if a template is used across multiple compilation units. This can be 
// resolved by declaring a template to extern and defining it only in a single
// compilation unit.
//
// Reference: http://blog.bitwigglers.org/extern-templates/

// Comment this out to see the weak reference Foo<int>(int) in extern1.o
extern template int Foo(int);

int f() {
   return Foo<int>( 3 );
}

int g();

int main() {
   // If you make extern and then:
   // 1) nm --demangle extern1.o
   // 2) nm --demangle extern2.o
   //
   // You can see in both object files:
   //   0000000000000000 W int Foo<int>(int)
   //
   // But if you use extern template, in extern1.o:
   //   U int Foo<int>(int)
   //
   // Now it is undefined, not wasting compiler time.
   std::cout << "f(): " << f() << std::endl;
   std::cout << "g(): " << g() << std::endl;
   return 0;
}

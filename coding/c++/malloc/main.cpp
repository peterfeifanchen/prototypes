#include <iostream>
#include <stdlib.h>

int main() {
   
   std::cout << "Malloc........" << std::endl;
   void *p1 = malloc( 1024 );
   std::cout << "p11: " << std::hex << p1 << std::endl; 
   
   size_t *s = ( size_t* ) p1;
   s = s - 1;
   std::cout << "s1: " << std::hex << s << std::endl;
   std::cout << "*s1 (header size 0x11 + allocated mem 0x400): 0x" << *s << std::endl;

   void *p2 = malloc( 2048 );
   std::cout << "p2: " << std::hex << p2 << std::endl; 
   
   s = ( size_t* ) p2;
   s = s - 1;
   std::cout << "s2: " << std::hex << s << std::endl;
   std::cout << "*s2 (header size 0x11 + allocated mem 0x800): 0x" << *s << std::endl;
   
   
   std::cout << std::endl << "New.........." << std::endl;
   void *n1 = new char[1024];
   std::cout << "p11: " << std::hex << n1 << std::endl; 
   
   s = ( size_t* ) n1;
   s = s - 1;
   std::cout << "s1: " << std::hex << s << std::endl;
   std::cout << "*s1 (header size 0x11 + allocated mem 0x400): 0x" << *s << std::endl;

   void *n2 = new char[2048];
   std::cout << "p2: " << std::hex << n2 << std::endl; 
   
   s = ( size_t* ) n2;
   s = s - 1;
   std::cout << "s2: " << std::hex << s << std::endl;
   std::cout << "*s2 (header size 0x11 + allocated mem 0x800): 0x" << *s << std::endl;
   
   return 0;
}


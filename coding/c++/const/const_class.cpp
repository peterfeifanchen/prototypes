#include<iostream>

class const_check{    
   int a;
 public:
   const_check(int i) : a(i) {}     
   void print() const {
      // a = 10  // This is a compiler error as print() is a const
      std::cout << "Const print(), value of a: " << a << std::endl;
   }
   
   void print() {
      a = 19;
      std::cout << "Non-Const print(), value of a: " << a << std::endl;
   }
};

int main(){
   const_check b(6);
   const const_check c(6);
   b.print(); //Calls non-const print()
   c.print(); //Calls const print()
}

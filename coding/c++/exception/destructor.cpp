#include <iostream>

class Bad {
 public:
   // Added the noexcept(false) so the code keeps its original meaning.
   // Post C++11 destructors are by default `noexcept(true)` and
   // this will (by default) call terminate if an exception is
   // escapes the destructor.
   //
   // But this example is designed to show that terminate is called
   // if two exceptions are propagating at the same time.
   ~Bad() noexcept(false) {
      throw 1;
   }          
};

class Bad2 {
 public:
   ~Bad2() {
      throw 1;   
   }
};

int main(int argc, char* argv[]) {
   try {
      Bad bad;
      // exception will call bad's destructor once we exit scope which throws.
      // It does not end up calling std::terminate because we set noexcept(false).
   } catch (...) {
      std::cout << "Caught exception" << std::endl;
   }

   /*
   try {
      Bad bad;
      throw 2;
      // Two exception propagating at the same time causes terminate to be called.
   } catch (...) {
      std::cout << "Will never catch this exception" << std::endl;
   }*/
   
   try {
      Bad2 bad;
      // noexcept(true) -> std::terminate
   } catch ( ... ) {
      std::cout << "Will never catch this exception" << std::endl;
   }
}

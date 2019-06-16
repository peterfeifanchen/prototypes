#include<iostream>
#include<string>

class Object {
   int i_;
 public:
   Object( int i ) : i_(i) {
      std::cout << "Creating Object" << i_ << std::endl;
   }

   ~Object() {
      std::cout << "Deleting Object" << i_ << std::endl;
   }
};

int main() {
   Object* strArray = new Object[5]{ 
      {0}, {1}, {2}, {3}, {4}
   };
   delete[] strArray;
}

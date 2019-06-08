#include<iostream>

class parent {
 public:
   virtual void print() {
      std::cout << "Parent print()" << std::endl;
   }
};

class child1 : public parent {
 public:
   void print() {
      std::cout << "Child1 print()" << std::endl;
   }
};

class child2 : public parent {
 public:
   void print() {
      std::cout << "Child2: ";
      parent::print();
   }
};

class abstractParent {
 public:
    virtual void print() = 0;
};

class child3 : abstractParent {
 public:
    void print() {
      std::cout << "Child3 print" << std::endl;
    }
};

int main() {
   //segfault -> late-binding cannot find the virtual function to call as type 
   //            is NULL
   //parent *p = NULL;
   //p->print();

   parent *p = new parent;
   p->print(); // Parent print()
   delete p;

   p = new child1;
   p->print(); // Child1 print()
   delete p;

   p = new child2;
   p->print(); // Child2: Parent print()
   delete p;

   // compiler error -> class with pure virtual functions cannot be instantiated
   //abstractParent *ap = new abstractParent;
}

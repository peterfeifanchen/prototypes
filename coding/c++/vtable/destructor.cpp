#include<iostream> 

using namespace std; 

class base { 
 public: 
   base() { cout<<"Constructing base \n"; } 
   virtual ~base() { cout<<"Destructing base \n"; }   
}; 

class derived: public base { 
 public: 
   derived() { cout<<"Constructing derived \n"; } 
   ~derived() { cout<<"Destructing derived \n"; } 
}; 

int main(void) 
{ 
   derived *d = new derived(); 
   base *b = d; 
   // if base destructor was not virtual, the derived destructor would not be called
   // here when you delete base *. It would still be called even if virtual was not
   // specified if you did delete derived * (calls both derived and base 
   // destructors).
   delete b; 
   return 0; 
} 



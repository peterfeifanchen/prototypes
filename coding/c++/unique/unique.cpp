#include <cstdlib>
#include <iostream>
#include <memory>              // unique_ptr
#include <string>
#include <vector>

class Clam{
 public:
   Clam(std::string &name) : name_(name) {
      std::cout << "Creating " << name_ << std::endl;
   }
            
   Clam(std::string &&name) : name_(std::move(name)) {
      std::cout << "Creating " << name_ << std::endl;
   }
               
   virtual ~Clam() { std::cout << "Destroying " << name_ << std::endl; }

   static std::unique_ptr<Clam>  generateClam( std::string, int );
                    
   std::string& printName() {
      return name_;
   }

 protected:
   std::string name_;
};

class Gigas : public Clam {
 public:
   Gigas(std::string &name) : Clam(name) {}
   Gigas(std::string &&name) : Clam(std::move(name)) {}
   ~Gigas() { std::cout << "Type Gigas "; }
};

class Durasa : public Clam {
 public:
   Durasa(std::string &name) : Clam(name) {}
   Durasa(std::string &&name) : Clam(std::move(name)) {}            
   ~Durasa() { std::cout << "Type Durasa "; }
};

class Squamosa : public Clam {
 public:
    Squamosa(std::string &name) : Clam(name) {}    
    Squamosa(std::string &&name) : Clam(std::move(name)) {}          
    ~Squamosa() { std::cout << "Type Squamosa "; }
};

std::unique_ptr<Clam> Clam::generateClam( std::string name, int type ) {
   switch(type) {
   case 0:
      return std::make_unique<Gigas>(name);
   case 1:
      return std::make_unique<Durasa>(name);
   default:
      return std::make_unique<Squamosa>(name);     
   }           
}


int main() {
   auto alice = std::make_unique<Clam>("Alice");
   auto bob = Clam::generateClam("Bob", rand() % 3 );
   //auto bob2 = bob; //Compiler error
   std::cout << "Before std::move, bob.name_: " << bob->printName() << std::endl;
   auto bob3 = std::move( bob ); //Transfer ownership of bob to bob3 

   // segfault as bob no longer exists
   // std::cout << "After std::move, bob.name_: " << bob->printName() << std::endl;
   if( bob == NULL ) std::cout << "After std::move, bob is now NULL" << std::endl;
   std::cout << "After std::move, bob3.name_: " << bob3->printName() << std::endl;

   std::vector<std::unique_ptr<Clam>> clamVec;

   clamVec.push_back( std::move( Clam::generateClam("Carl", rand() % 3) ) );               
   clamVec.push_back( std::move( Clam::generateClam("Dave", rand() % 3) ) );                 
   clamVec.push_back( std::move( Clam::generateClam("Ernst", rand() % 3) ) );
                     
   return 0;
}

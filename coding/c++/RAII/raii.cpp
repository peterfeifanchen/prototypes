#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

class Octopus {
 public:
   Octopus( std::string &name ) : name_( name ) {            
      std::cout << "Creating " << name_ << std::endl;                 
   }
            
   Octopus( std::string &&name ) : name_( std::move( name ) ) {
      std::cout << "Creating " << name_ << std::endl;
   }

   ~Octopus() { std::cout << "Destroying " << name_ << std::endl; }

   std::string name_;
};

class OctopusOwner {
 public:
   // Acquires the Octopus resource on instantiation
   OctopusOwner( Octopus *octopus ) : 
      octopus_( std::move( std::unique_ptr<Octopus>( octopus ) ) ) {}
   
   // Deletes the octopus resource on scope exit
   ~OctopusOwner() { octopus_.reset(nullptr); }
               
   void useOctopus() {
      std::cout << "Using octopus " << octopus_->name_ << std::endl;   
   }

 private:               
   std::unique_ptr<Octopus> octopus_;
};


int main() {   
   Octopus *alice = new Octopus("Alice");     
   OctopusOwner aliceOwner( alice );
   aliceOwner.useOctopus();
   if( alice != NULL ) 
      std::cout << "alice ptr is not NULL as it wasn't created as an unique_ptr" << std::endl;

   // This is preferred to the above code where a reference exists elsewhere for
   // alice. In the alice case, another control path may delete the memory even
   // though our aliceOwner "technically" suppose to have it. For things like
   // files, you can even open it in the constructor and close it in the destructor.
   OctopusOwner bobOwner( new Octopus("Bob") );
   bobOwner.useOctopus();

   return 0;
}

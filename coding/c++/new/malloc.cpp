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
   Object* strArray = static_cast<Object*>( malloc( sizeof(Object) * 5 ) );
   
   // Unfortunately, there is no really good way to instantiate the Objects
   // themselves after you've allocated memory for it. One method is to use
   // placement new.
   for( Object* cur = strArray; cur < strArray+5; cur++ ) {
      //std::cout << "cur " << std::hex << cur << " strArray " << std::hex 
      //      << strArray << std::endl;
      //std::cout << "size: " << sizeof(Object) << std::endl;
      //std::cout << (cur-strArray) << std::endl;
      // NOTE: compiler is smart to realize that cur - strArray is in units of
      //       Object, not bytes.
      new (cur) Object( cur - strArray  );
   }

   for( Object* cur = strArray; cur < strArray+5; cur++ ) {
      cur->~Object();
   }
   
   free( strArray );
}

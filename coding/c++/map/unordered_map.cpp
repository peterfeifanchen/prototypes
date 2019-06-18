#include<iostream>
#include<unordered_map>
#include<string>
#include<functional>  

typedef std::pair<std::string, std::string> Name;

// Overload operator== globally, as std::pair have its own operator==, we don't need
// to worry as long as the types in std::pair have their operator==;
class Key{
   int a_; 
 public:
   Key( int a ) : a_(a) {}

   int a() const { return a_; }
};

struct equalClass{
   bool operator()( const Key& lhs, const Key& rhs ) const {
      return lhs.a() == rhs.a();
   }
};

struct keyHash{
   size_t operator()( const Key& k ) const {
      return std::hash<int>()( k.a() );
   }
};

bool operator==( const Key& lhs, const Key& rhs ) {
   return equalClass()( lhs, rhs );
}

// Template specialize hash<key>
namespace std {
   template <>
   class hash<Name>{
    public:
      size_t operator()(const Name &name ) const {                                        
         return hash<string>()(name.first) ^ hash<string>()(name.second);                                            
      }
   };
};

// Custom hash class
struct hashClass {
   size_t operator()( const Name &name ) const {
      return std::hash<std::string>()( name.first ) ^ 
         std::hash<std::string>()( name.second );
   }
};

// Custom hash function
size_t hashName( const Name &name ) {
   // Use hash<key> class to create your hash using a XOR
   return std::hash<std::string>()( name.first ) ^ 
      std::hash<std::string>()( name.second );
}

std::ostream& operator<<( std::ostream &os, const Key& k ) {
   os << k.a();
   return os;
}

std::ostream& operator<<( std::ostream &os, const Name& name ) {
   os << name.first << " " << name.second;
   return os;
}

int main() {
   // Basic unordered map
   std::cout << "Use default hash" << std::endl;
   std::unordered_map<std::string, int> id1;
   id1["abcde"] = 100;
   id1["edcba"] = 200;
   for( auto it = id1.begin(); it != id1.end(); it++ ) {
      std::cout << "key: " << it->first << " value: " << it->second << std::endl;
   }

   std::cout << "\nUsing custom function as hash" << std::endl;
   // Unordered map where you have to supply hash function. Note we must use the
   // constructor that initializes 100 buckets and gives a hashing function along
   // with passing the type of the function as a template class parameter in place of
   // the default hash<key> class. 
   std::unordered_map<Name, int, decltype(&hashName) > id2( 100, hashName );
   id2[ Name( "Peter", "Chen" ) ] = 12345;
   id2[ Name( "Fifi", "Chen" ) ] = 23456;
   for( auto it = id2.begin(); it != id2.end(); it++ ) {
      std::cout << "key: " << it->first << " value: " << it->second << std::endl;
   }

   std::cout << "\nUsing lambda function as hash" << std::endl;
   // Unordered map where you have to supply hash function as a lambda.
   std::unordered_map<Name, int, std::function< size_t ( const Name &name )>> id3( 100,
         []( const Name &name ) {
            // Use hash<key> class to create your hash using a XOR
            return std::hash<std::string>()( name.first ) ^ 
               std::hash<std::string>()( name.second );
         } );
   id3[ Name( "Peter", "Chen" ) ] = 12345;
   id3[ Name( "Fifi", "Chen" ) ] = 23456;
   for( auto it = id3.begin(); it != id3.end(); it++ ) {
      std::cout << "key: " << it->first << " value: " << it->second << std::endl;
   }

   std::cout << "\nUsing a custom class as hash" << std::endl;
   // Unordered map where custom class is hash. Note that with template we don't need
   // to use the more specialized constructors.
   std::unordered_map<Name, int, hashClass> id4;
   id4[ Name( "Peter", "Chen" ) ] = 12345;
   id4[ Name( "Fifi", "Chen" ) ] = 23456;
   for( auto it = id4.begin(); it != id4.end(); it++ ) {
      std::cout << "key: " << it->first << " value: " << it->second << std::endl;
   }

   std::cout << "\nTemplate specialize to overload hash<T> as hash " << std::endl;
   std::unordered_map<Name, int> id5;
   id5[ Name( "Peter", "Chen" ) ] = 12345;
   id5[ Name( "Fifi", "Chen" ) ] = 23456;
   for( auto it = id5.begin(); it != id5.end(); it++ ) {
      std::cout << "key: " << it->first << " value: " << it->second << std::endl;
   }

   std::cout << "\nCustom ==operator for unordered_map " << std::endl;
   // Unordered map where we use class Key as key and have to implement our own
   // operator==
   std::unordered_map<Key, int, keyHash, equalClass> id6;
   id6[ Key( 1 ) ] = 12345;
   id6[ Key( 2 ) ] = 23456;
   for( auto it = id6.begin(); it != id6.end(); it++ ) {
      std::cout << "key: " << it->first << " value: " << it->second << std::endl;
   }
   
   // Uses Key's global operator==
   std::unordered_map<Key, int, keyHash> id7;
   id7[ Key( 1 ) ] = 12345;
   id7[ Key( 2 ) ] = 23456;
   for( auto it = id7.begin(); it != id7.end(); it++ ) {
      std::cout << "key: " << it->first << " value: " << it->second << std::endl;
   }
}


#include <iostream>
#include <type_traits>

int main( int argc, char** argv ) {
   std::cout << std::is_trivially_copyable<long long int>::value << std::endl;
   std::cout << std::is_trivially_copyable<int>::value << std::endl;
   return 0;
}


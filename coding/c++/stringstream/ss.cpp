#include <iostream>
#include <sstream>
 
int main() {
   std::stringstream ss( "" );
   
   // Writing to string stream.s
   // 40H
   ss << 40 << "H";
   std::cout << ss.str() << std::endl;
  
   // Reading an int from string stream
   // a = 40
   int a;
   ss >> a;
   std::cout << "a: " << a << std::endl;
   
   // Writing to string stream at the end
   // 40H50,
   ss << a + 10 << ",";
   std::cout << ss.str() << std::endl;
 
   // Reset stream to start.
   ss.seekg(0, ss.beg);
   ss >> a;
   std::cout << "a: " << a << std::endl;
    
   // Implicit conversion of istream to bool achieved by looking at ss.fail().
   // Reading another int would fail as "H" cannot be parsed into an int.
   bool valid = static_cast<bool>( ss >> a );
   std::cout << "Reading form string stream: " << valid << std::endl;
   if ( ss.fail() ) std::cout<< "Reading from string stream failed" << std::endl;

   return 0;
}

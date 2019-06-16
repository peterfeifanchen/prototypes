#include<iostream>

int gcd( int a, int b ) {
   if( b == 0 ) return a;
   return gcd( b, a % b );
}


int main() {
   int a = 125;
   int b = 25;

   std::cout << "a: " << a << " b: " << b << " gcd: " << gcd(a, b) << std::endl;

   a = 125;
   b = 36;

   std::cout << "a: " << a << " b: " << b << " gcd: " << gcd(a, b) << std::endl;
   
   a = 125;
   b = 75;

   std::cout << "a: " << a << " b: " << b << " gcd: " << gcd(a, b) << std::endl;
}


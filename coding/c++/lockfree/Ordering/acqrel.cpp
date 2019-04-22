#include <atomic>

typedef long long int int64;

int main( int argc, char** argv) {
   std::atomic<int64> aVal1;

   aVal1.store( 10, std::memory_order_release );
   int64 tmp = aVal1.load( std::memory_order_acquire );
   aVal1.store( tmp + 10, std::memory_order_release );

   return 0;
}


#include <atomic>

typedef long long int int64;

int main( int argc, char** argv) {
   std::atomic<int64> aVal1;

   aVal1.store( 10, std::memory_order_seq_cst );
   int64 tmp = aVal1.load( std::memory_order_seq_cst );
   aVal1.store( tmp + 10, std::memory_order_seq_cst );

   return 0;
}


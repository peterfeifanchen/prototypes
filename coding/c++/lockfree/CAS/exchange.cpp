#include<iostream>
#include<atomic>
#include<thread>
#include<vector>

// There is std::exchange and std::atomic_exchange. They differ in their function
// signatures.
//
// We use std::exchange to implement a spin lock. The spin lock has a value "true"
// if it is locked and "false" if it is unlocked. We do an experiment where we 
// launch a thousand c++ threads, each incrementing the value of a variable by 1.
// The end result should be 1000 if the spin lock is correctly used/implemented.

struct Lock{

   std::atomic<bool> &l;
   Lock( std::atomic<bool> &l_ ) : l(l_) {
      lock();
   }

   ~Lock() {
      unlock();
   }

   void lock() {
      // spin until lock is relinquished
      while( std::atomic_exchange_explicit( &l, true, std::memory_order_acquire ) );
   }

   void unlock() {
      std::atomic_store_explicit( &l, false, std::memory_order_release );
   }
};

void f( int &n, std::atomic<bool> &l ) {
   Lock lock( l );
   n++;
   // destructor will call unlock
}

int main() {
   std::vector<std::thread> v;
   int n = 0; 
   std::atomic<bool> lock(false);

   for( int i = 0; i < 1000; i++ ) {
      v.emplace_back( f, std::ref(n), std::ref(lock) ); 
      //v.emplace_back( f, &n );
   }

   for( auto &t : v ) {
      t.join();
   }

   if( n == 1000 ) {
      std::cout << "Spin lock worked!" << std::endl;
   } else {
      std::cout << "Got " << n << " :(." << std::endl;
   }

   return 0;
}

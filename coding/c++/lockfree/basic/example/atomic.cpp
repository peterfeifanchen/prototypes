// Copyright (c) 2019 Arista Networks, Inc.  All rights reserved.
// Arista Networks, Inc. Confidential and Proprietary.

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include <sys/syscall.h>
#include <mutex>

#define OCCUPIED            0x1
#define LOOKASIDE           0x2
#define unlikely(x)         __builtin_expect( (x), 0 )
#define gettid()            syscall(SYS_gettid)
#define getcpu(a, b, c)     syscall(SYS_getcpu, a, b, c)
#define COMPILER_BARRIER()  asm volatile( "" : : : "memory" )

typedef long long int int64;

// synchronization primitives for controlling experiment
std::mutex c_out_mutex;

// CPU, Node data for reader/writer threads
unsigned writerCpu, writerNode, readerCpu, readerNode;

struct dataType {
   int64 value1_ = 0;
   int64 value2_ = 0;
};

std::ostream& operator<<(std::ostream& os, const dataType& d) {
   os << "value1_: " << d.value1_ << " value2_: " << d.value2_;
   return os;
}

struct dataType buffer;

class LockFreeType {
 
 std::atomic<int> version_;
 // atomic can only be applied on types that are trivially copyable!
 std::atomic<dataType> value_;

 public:   
   void store( const dataType &value, dataType &buf );

   dataType load( const dataType &buf ) const;
};

// LockFreeType::store looks at the storage location to check if it is occupied.
// This is indicated by an odd version_. 
//
// If occupied, value is copied to the given buf first. Then because all 32-bit
// operations are atomic by default, the version_ is incremented. This atomically
// switches reads from other threads to the global buf. In the background, the 
// writer thread then continues to copy value into the true location value_.
void LockFreeType::store( const dataType &value, dataType &buf ) {
#ifdef MEMORY_ORDER
   if( version_.load( std::memory_order_acquire ) & OCCUPIED ) {
      buf = value;
   }
   int tmp = version_.load( std::memory_order_acquire );
   version_.store( tmp + LOOKASIDE , std::memory_order_release );
   value_.store( value, std::memory_order_release );
   int localVersion = ( version_.load( std::memory_order_acquire ) 
            + LOOKASIDE) | OCCUPIED;
   version_.store( localVersion, std::memory_order_release );
#else
   if( version_.load( std::memory_order_relaxed ) & OCCUPIED ) {
      buf = value;
   }
   int tmp = version_.load( std::memory_order_relaxed );
   version_.store( tmp + LOOKASIDE , std::memory_order_relaxed );
   value_.store( value, std::memory_order_relaxed );
   int localVersion = ( version_.load( std::memory_order_relaxed ) 
            + LOOKASIDE) | OCCUPIED;
   //version_.store( localVersion, std::memory_order_release );
   version_.store( localVersion, std::memory_order_relaxed );
#endif
}

// LockFreeType::load keeps reading the value, either out of buf or from value_
// until the version number matches. This ensures an atomic value is returned 
// (note: it does not return the latest value). A mismatch indicates that there
// may be a writer writing concurrently at the address that a reader is reading.
dataType LockFreeType::load( const dataType &buf ) const {
   int origVersion;
   dataType value;
#ifdef MEMORY_BARRIER
   do {
      origVersion = version_.load( std::memory_order_acquire );
      // If writes are unlikely compared to reads then LOOKASIDE will most likely 
      // not be set.
      if( unlikely( origVersion & LOOKASIDE ) ) {
         value = buf;
      } else {
         value = value_;
      }
   } while( origVersion != version_.load( std::memory_order_acquire ) );
#else
   do {
      origVersion = version_.load( std::memory_order_relaxed );
      // If writes are unlikely compared to reads then LOOKASIDE will most likely 
      // not be set.
      if( unlikely( origVersion & LOOKASIDE ) ) {
         value = buf;
      } else {
         value = value_;
      }
   } while( origVersion != version_.load( std::memory_order_relaxed ) );
#endif
   return value;
}

void *lfWriter( void* arg ) {
   LockFreeType *lf = ( LockFreeType* ) arg;
   dataType v;
   v.value1_ = 200;
   v.value2_ = 200;
   int count = 0;
   int operation = 0; //0 is add, 1 is subtract. So that our add won't overflow.
   
   cpu_set_t cpus;
   CPU_ZERO( &cpus );
   CPU_SET( 30, &cpus );
   pid_t pid = gettid();
   sched_setaffinity( pid, sizeof(cpu_set_t), &cpus );
   getcpu( &writerCpu, &writerNode, NULL );
   {
      std::lock_guard<std::mutex> lock(c_out_mutex);
      std::cout << "Writer " << pid << ": cpu " << writerCpu << " node " 
         << writerNode << std::endl;
   }

   while( count < 10 ) {
      lf->store( v, buffer );
      count++;
      if( count == 5 ){
         count = 0;
         operation += 1;
      }
      if( operation & 0x1 ) {
         v.value1_ += 10;
         v.value2_ += 10;
      } else {
         v.value1_ -= 10;
         v.value2_ -= 10;
      }
   }
}

int main( int argc, char** argv ) {
   LockFreeType lfType;
   
   // NOTE: This lockfree example only works for single-writer multiple-readers.
   pthread_t thread1;
   pthread_create( &thread1, NULL, lfWriter, (void*) &lfType );
   
   cpu_set_t cpus;
   CPU_ZERO( &cpus );
   CPU_SET( 0, &cpus );
   pid_t pid = gettid();
   sched_setaffinity( pid, sizeof(cpu_set_t), &cpus );
   getcpu( &readerCpu, &readerNode, NULL );
   {
      std::lock_guard<std::mutex> lock(c_out_mutex);
      std::cout << "Reader " << pid << ": cpu " << readerCpu << " node " 
         << readerNode << std::endl;
   }

   int64 numReads = 0;
   int64 readErrors = 0;
   while( true ) {
      dataType v = lfType.load( buffer );   
      numReads++;
      // If atomic write was correct, then v.value1_ == v.value2_ 
      if( v.value1_ != v.value2_) {
         readErrors++;
         std::cout << "Reader: " << v << " " << readErrors << "/" << numReads 
            << " errors" << std::endl;
      }
   }
}

#include <iostream>
#include <pthread.h>
#include <unistd.h>
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
 
 int version_ = 0;
 dataType value_;

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
   if( version_ & OCCUPIED ) {
      buf = value;
   }
   // Between version_ and value_ there is no memory dependency, a weak ordering CPU
   // can re-order these two operations leading to the reader reading a half-filled
   // from value_ instead of being redirected to buffer. However, we were unable to 
   // get this error to show up. 
   version_ += LOOKASIDE;
   // Compiler barriers are necessary here because of compiler reordering and
   // optimizations that may combine the two adds to version_ into one at the 
   // end (e.g. version_ = ( version_ + LOOKASIDE | OCCUPIED ) + LOOKASIDE. 
   COMPILER_BARRIER();
   value_ = value;
   int localVersion = ( version_ + LOOKASIDE) | OCCUPIED;
   // Compiler barriers are necessary here to avoid optimizing out localVersion.
   COMPILER_BARRIER();
   version_ = localVersion;
}

// LockFreeType::load keeps reading the value, either out of buf or from value_
// until the version number matches. This ensures an atomic value is returned 
// (note: it does not return the latest value). A mismatch indicates that there
// may be a writer writing concurrently at the address that a reader is reading.
dataType LockFreeType::load( const dataType &buf ) const {
   int origVersion;
   dataType value;
   do {
      COMPILER_BARRIER();
      origVersion = version_;
      // If writes are unlikely compared to reads then LOOKASIDE will most likely 
      // not be set.
      if( unlikely( origVersion & LOOKASIDE ) ) {
         value = buf;
      } else {
         value = value_;
      }
      COMPILER_BARRIER();
   } while( origVersion != version_ );

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
#ifdef SAMENODE
   CPU_SET( 0, &cpus );
#else
   CPU_SET( 30, &cpus );
#endif
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

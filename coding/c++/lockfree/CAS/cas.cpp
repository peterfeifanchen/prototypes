#include <iostream>
#include <semaphore.h>
#include <chrono>
#include <random>
#include <pthread.h>
#include <thread>
#include <sched.h>
#include <atomic>
#include <unistd.h>
#include <sys/syscall.h>

#define gettid() syscall(SYS_gettid)
#define getcpu(a, b, c) syscall(SYS_getcpu, a, b, c)

struct Item {
   int   value;
   Item* next;
   Item( int value_, Item* next_) : value(value_), next(next_) {}
  
   // CAS comparison is bitwise comparison similar to std::memcmp. Comparison operators
   // are not used. 
   bool operator==( Item &rhs ) {
      std::cout <<"Compare" << std::endl;
      std:: cout << "  value: " << this->value << " next: " << this->next << std::endl;
      std:: cout << "   value: " << rhs.value << " next: " << rhs.next << std::endl;
      if( this->value == rhs.value && this->next == rhs.next ) return true;
      return false;
   }
};

struct MemorySlot {

   Item* item;
    
   MemorySlot() {
      item = NULL;
   }

   void NewSlot( int val, Item* next ) {
      if( item == NULL ) item = new Item( val, next);
      else *item = Item( val, next);
   }

    void DeleteSlot() {
       if( item != NULL ) {
          delete item;
         item = NULL;
       }
    }

    ~MemorySlot() {
      if( item != NULL ) delete item; 
    }
};
    
// We illustrate compare_and_swap_weak with an example that reveals its ABA use-after-free
// problem. 
//
// Thread 1: pop the head
//           wait random amount of time
//           make the next element the head
//
// Thread 2: pop the head
//           pop the head
//           add a new item with value of 1 
//           
// Main Thread: create linked-list of queue of 4 elements, 1 -> 2 -> 3 -> 4 
//              tell thread 1 to go
//              tell thread 2 to go
//              check the head if it wasn't already freed (ABA if already freed). 
//              restore queue correctness 
//              repeat

std::atomic<Item*> head(NULL);
unsigned thread1Cpu, thread1Node, thread2Cpu, thread2Node;
MemorySlot A, B, C, D;
sem_t sem1, sem2, restart, thread1proceed;

void* thread1Func( void* param ) {
   unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
   std::mt19937 g1( seed ); //MersenneTwister PSN
   cpu_set_t cpus;
   CPU_ZERO( &cpus );
   CPU_SET( 0, &cpus );
   pid_t pid = gettid();
   sched_setaffinity( pid, sizeof(cpu_set_t), &cpus );
   getcpu( &thread1Cpu, &thread1Node, NULL );

   for (;;) {
      sem_wait( &sem1 );
  
      // atomic are not CopyAssignable, but provides a conversion operator T() which
      // implicitly converts atomic to its type T. This is called indirectly by 
      // static_cast<Item*>. Combined with the assignment operator = which takes in
      // T and converts it into a store(T) operation. 
      Item* currHead;
      while( 1 ) {
         currHead = static_cast<Item*>(head);
         // NOTE: we can do 
         // std::atomic_compare_exchange_weak( head, currHead, currHead->next)
         // but this would be equivalent to the following, but just making it
         // more difficult to generate the interleaving (cutting between 
         // currHead->next and the CAS operation)
         Item* next = currHead->next;
         // Uncomment below to make it happen everytime
         sleep(1);
         while( g1() % 8 != 0 ) {} //short random delay
         if( head.compare_exchange_weak( currHead, next, std::memory_order_release,
                  std::memory_order_relaxed ) )
            break;
      }

      sem_post( &restart );
   }
}

void* thread2Func( void* param ) {
   cpu_set_t cpus;
   CPU_ZERO( &cpus );
   CPU_SET( 1, &cpus );
   pid_t pid = gettid();
   sched_setaffinity( pid, sizeof(cpu_set_t), &cpus );
   getcpu( &thread2Cpu, &thread2Node, NULL );

   MemorySlot* m = NULL;
   Item* currHead;

   for (;;) {
      sem_wait( &sem2 );

      // pop head
      while( 1 ) {
         currHead = static_cast<Item*>(head);
         if( head.compare_exchange_weak( currHead, currHead->next, 
                  std::memory_order_release, std::memory_order_relaxed) )
            break;
      }

      // pop head
      while( 1 ) {
         currHead = static_cast<Item*>(head);
         if( head.compare_exchange_weak( currHead, currHead->next, 
                  std::memory_order_release, std::memory_order_relaxed) )
            break;
      }
      currHead->next = NULL;
      
      // push A if C is still head
      while( 1 ) {
         currHead = head.load( std::memory_order_seq_cst );
         // This is where depending on the allocator, a new memory block that is 
         // exactly the same as the freed memory block A that was previously freed
         // would be chosen. CAS would then memcmp the pointers to TRUE.
         A.NewSlot( 1, currHead ); 
         if( head.compare_exchange_weak( currHead, A.item, 
                  std::memory_order_release, std::memory_order_relaxed) ) break;
      }

      sem_post( &restart );
   }
}

std::ostream& operator<<( std::ostream& os, const Item* i ) {
   switch( i->value ) {
      case 1: os << "A";
              break;
      case 2: os << "B";
              break;
      case 3: os << "C";
              break;
      case 4: os << "D";
              break;
      default: os << i->value;
   }

   return os;
}

int main( int argc, char** argv ) {
  
   pthread_t thread1, thread2;
   pthread_create( &thread1, NULL, thread1Func, NULL );
   pthread_create( &thread2, NULL, thread2Func, NULL );

   int ABA = 0;

   for( int iteration=1;; iteration++ ) {
      D.NewSlot( 4, NULL );
      C.NewSlot( 3, D.item );
      B.NewSlot( 2, C.item );
      A.NewSlot( 1, B.item );
      head.store( A.item, std::memory_order_seq_cst );

      // Signal both threads to begin contention
      // head->A->B->C->D
      sem_post( &sem1 );
      sem_post( &sem2 );

      // Wait for end of contention
      sem_wait( &restart );
      sem_wait( &restart );

      // Fake B MemorySlot remove. If this was a real remove, this would have been
      // a use-after-free scenario which would be extraordinarily hard to understand 
      // and debug.
      if( static_cast<Item*>(head)->next == NULL ) {
         std::cout << "Head Value: " << static_cast<Item*>(head) 
                   << " (freed)" << std::endl;
         std::cout << "ABA race detected " << ABA++ << "/" << iteration << std::endl;
      }

      A.DeleteSlot();
      B.DeleteSlot();
      C.DeleteSlot();
      D.DeleteSlot();
   }
}


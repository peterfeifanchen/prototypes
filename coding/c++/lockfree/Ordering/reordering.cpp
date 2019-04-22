#include <iostream>
#include <semaphore.h>
#include <chrono>
#include <random>
#include <pthread.h>
#include <sched.h>
#include <atomic>
#include <unistd.h>
#include <sys/syscall.h>
#include <mutex>

#define gettid() syscall(SYS_gettid)
#define getcpu(a, b, c) syscall(SYS_getcpu, a, b, c)

// synchronization primitives for controlling the experiment
sem_t sem1, sem2, restart;
std::mutex c_out_mutex;

// CPU, Node data for each thread
unsigned thread1Cpu, thread1Node, thread2Cpu, thread2Node;

// shared variables
std::atomic<int> X, Y, r1, r2;

void *thread1Func( void *param ) {
   unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
   std::mt19937 g1( seed ); // MersenneTwister pseudo-random generator
		
   cpu_set_t cpus;
   CPU_ZERO( &cpus );
   CPU_SET( 0, &cpus );
   pid_t pid = gettid();
   sched_setaffinity( pid, sizeof(cpu_set_t), &cpus );
   getcpu( &thread1Cpu, &thread1Node, NULL);
   // std::cout is not thread-safe, need to lock
   {
      std::lock_guard<std::mutex> lock(c_out_mutex);
      std::cout << "Thread1 " << pid << ": cpu " << thread1Cpu << " node " 
         << thread1Node << std::endl;
   }

   for (;;) {
	  sem_wait( &sem1 );
	  while( g1() % 8 != 0 ) {} // Short random delay

	  // -------write X, read Y---------------------
      #ifdef ACQREL
      // Prevent reodering with X load as memory_order_acquire semantics can be 
      // apparently applied to both stores and loads whereas memory_order_release
      // semantics are only applied to stores. Further while memory_order_seq_cst
      // and memory_order_acq_rel can also be applied. This was apparently not
      // necessary. Similarly release or acquire semantics are not required for Y
      // load. Finally, in this case, apparently is enough to stop memory 
      // reordering. 
      X.store( 1, std::memory_order_acquire );
      #else
	  X.store( 1, std::memory_order_relaxed );
      #endif
      
      #ifdef MFENCE
      // Prevents memory reordering
      asm volatile( "mfence" ::: "memory" );
      #elif ACQRELFENCE
      // Prevents acq-rel reordering - insufficient in this case as we have a 
      // storeload relationship. 
      std::atomic_thread_fence(std::memory_order_acq_rel);
      #else
	  // Prevents compiler Reordering
	  asm volatile( "" ::: "memory" );
      #endif
	  
      r1 = Y.load( std::memory_order_relaxed );

      sem_post( &restart );
   }

   return NULL;
}

void *thread2Func( void *param ) {
   unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
   std::mt19937 g1( seed ); // MersenneTwister pseudo-random generator
        
   cpu_set_t cpus;
   CPU_ZERO( &cpus );
   #ifdef SAME_NODE
   // Choose a CPU on the same node as thread1. On my machien this was cpu 1.
   CPU_SET( 0, &cpus );
   #else
   // Choose a CPU on a different node as thread1. On my machine this was cpu 30.
   CPU_SET( 30, &cpus );
   #endif
   pid_t pid = gettid();
   sched_setaffinity( pid, sizeof(cpu_set_t), &cpus );
   getcpu( &thread2Cpu, &thread2Node, NULL);
   {
      std::lock_guard<std::mutex> lock(c_out_mutex);
      std::cout << "Thread2 " << pid << ": cpu " << thread2Cpu << " node " 
         << thread2Node << std::endl;
   }

   for (;;) {
      sem_wait( &sem2 );
	  while( g1() % 8 != 0 ) {} // Short random delay to allow thread interleaving
				
	  // -------write Y, read X---------------------
      #ifdef ACQREL
      // Prevent reodering with X load as memory_order_acquire semantics can be 
      // apparently applied to both stores and loads whereas memory_order_release
      // semantics are only applied to stores. Further while memory_order_seq_cst
      // and memory_order_acq_rel can also be applied. This was apparently not
      // necessary. Similarly release or acquire semantics are not required for X
      // load. Finally, in this case, apparently is enough to stop memory 
      // reordering. 
      Y.store( 1, std::memory_order_acquire );
      #else
	  Y.store( 1, std::memory_order_relaxed );
      #endif

      #ifdef MFENCE
      // Prevents memory reordering
      asm volatile( "mfence" ::: "memory" );
      #elif ACQRELFENCE
      // Prevents acq-rel reordering - insufficient in this case as we have a 
      // storeload relationship. 
      std::atomic_thread_fence(std::memory_order_acq_rel);
      #else
      // Prevents compiler Reordering
	  asm volatile( "" ::: "memory" ); 
      #endif
      
	  r2 = X.load( std::memory_order_relaxed );
				
	  sem_post( &restart );
   }

   return NULL;
}

int main() {
   // Initialize the semaphores
   sem_init( &sem1, 0, 0 );
   sem_init( &sem2, 0, 0 );
   sem_init( &restart, 0, 0 );

   // Spawn threads
   pthread_t thread1, thread2;
   pthread_create( &thread1, NULL, thread1Func, NULL );
   pthread_create( &thread2, NULL, thread2Func, NULL );

   sleep(10);

   int detected = 0;
   for( int iteration=1;; iteration++ ) {
      // We reset x=0, y=0 to repeat the experiment until we detect
	  // reordering
	  X.store( 0, std::memory_order_relaxed );
	  Y.store( 0, std::memory_order_relaxed );

      // Signal both threads to begin contention
	  sem_post( &sem1 );
	  sem_post( &sem2 );

	  // Wait for end of contention
	  sem_wait( &restart );
	  sem_wait( &restart );

	  if( r1 == 0 && r2 == 0 ) {
	     detected++;
         std::cout << "Thread1: cpu " << thread1Cpu << " node " 
             << thread1Node << std::endl;
         std::cout << "Thread2: cpu " << thread2Cpu << " node " 
            << thread2Node << std::endl;
	  		std::cout << detected << " reorders detected after " << iteration << 
						" iterations" << std::endl;
	  }
   }
   return 0;
}

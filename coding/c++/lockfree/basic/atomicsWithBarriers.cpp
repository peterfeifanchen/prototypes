#include <iostream>
#include <chrono>
#include <random>
#include <pthread.h>
#include <atomic>

sem_t sem1, sem2, restart;

int X, Y, r1, r2;

void *thread1Func( void *param ) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::mt19937 g1( seed ); // MersenneTwister pseudo-random generator
		for (;;) {
				sem_wait( &sem1 );
				while( g1() % 8 != 0 ) {} // Short random delay
		
				// -------write X, read Y---------------------

				X.store( 1, std::memory_order_relaxed )
				asm volatile( "" ::: "memory" ); 
				r1.load( Y, std::memory_order_relaxed )
				
				// Prevents Memory Reordering
				// A full memory fence will prevent StoreLoad reordering in arch and 
				// eliminate X=0,Y=0 state.
				// on X, Y, r1, r2.
				// X = 1;
				// X.store( 1, std::memory_order_relaxed )
				// X.store( 1, std::memory_order_seq_cst )
				// asm volatile( "mfence" ::: "memory" ); 
				// r1 = Y;
				// r1.load( Y, std::memory_order_relaxed )
				// r1.load( Y, std::memory_order_seq_cst )
	
				sem_post( &restart );
		}

		return NULL;
}

void *thread2Func( void *param ) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::mt19937 g1( seed ); // MersenneTwister pseudo-random generator
		for (;;) {
				sem_wait( &sem1 );
				while( g1() % 8 != 0 ) {} // Short random delay to allow thread interleaving
		
				// -------write Y, read X---------------------

				// Prevents Compiler Reordering
				// In this mode, X86/64 arch does not guarantee that there will be 
				// sequential consistency between thread1Func and thread2Func's operations
				// on X, Y, r1, r2.
				Y = 1;
				asm volatile( "" ::: "memory" ); 
				r2 = X;
				
				// Prevents Memory Reordering
				// A full memory fence will prevent StoreLoad reordering in arch and 
				// eliminate X=0,Y=0 state.
				// on X, Y, r1, r2.
				//Y = 1;
				//asm volatile( "mfence" ::: "memory" );
				//std::atomic_thread_fence( std::memory_order_seq_cst )
				//r2 = X;
	
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
	pthread_t thread1, thread;
	pthread_create( &thread1, NULL, thread1Func, NULL );
	pthread_create( &thread2, NULL, thread2Func, NULL );

	// Pin the two threads on separate cores. If they run on the same core, then 
	// memory operations on X, Y, r1, r2 will exhibit sequential consistency and
	// state X=0,Y=0 will not occur.
	cpu_set_t cpus;
	CPU_ZERO( &cpus );
	CPU_SET( 0, &cpus );
	pthread_setaffinity_np( thread1, sizeof(cpu_set_t), &cpus );
	phtread_setaffinity_np( thread2, sizeof(cpu_set_t), &cpus );

	// We reset x=0, y=0 to repeat the experiment until we detect
	// reordering
	int detected = 0;
	for( int iteration=1;; iteration++ ) {
			// Reset X, Y
			X = 0;
			Y = 0;

			// Signal both threads to begin contention
			sem_post( &sem1 );
			sem_post( &sem2 );

			// Wait for end of contention
			sem_wait( &restart );
			sem_wait( &restart );

			if( r1 == 0 && r2 == 0 ) {
					detected++;
					std::cout << detected << " reorders detected after " << iteration << 
						" iterations" << std::endl;
			}
	}

	return 0;
}

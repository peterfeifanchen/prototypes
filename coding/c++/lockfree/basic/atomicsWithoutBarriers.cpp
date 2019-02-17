#include <iostream>
#include <semaphore.h>
#include <chrono>
#include <random>
#include <pthread.h>
#include <sched.h>
#include <atomic>

// synchronization primitives for controlling the experiment
sem_t sem1, sem2, restart;

// shared variables
std::atomic<int> X, Y, r1, r2;

void *thread1Func( void *param ) {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::mt19937 g1( seed ); // MersenneTwister pseudo-random generator
		for (;;) {
				sem_wait( &sem1 );
				while( g1() % 8 != 0 ) {} // Short random delay

				// -------write X, read Y---------------------
				X.store( 1, std::memory_order_relaxed );
				asm volatile( "" ::: "memory" ); 
				r1 = Y.load( std::memory_order_relaxed );

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
				Y.store( 1, std::memory_order_relaxed );
				// Prevents Compiler Reordering
				asm volatile( "" ::: "memory" ); 
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

	// Pin the two threads on separate cores. If they run on the same core, then 
	// memory operations on X, Y, r1, r2 will exhibit sequential consistency and
	// state X=0,Y=0 will not occur.
	cpu_set_t cpus1;
	cpu_set_t cpus2;
	CPU_ZERO( &cpus1 );
	CPU_ZERO( &cpus2 );
	CPU_SET( 0, &cpus1 );
	CPU_SET( 1, &cpus2 );
	// X86/64 arch does not guarantee that there will be sequential consistency 
	// between thread1Func and thread2Func's operations on X, Y, r1, r2.
	pthread_setaffinity_np( thread1, sizeof(cpu_set_t), &cpus1 );
	phtread_setaffinity_np( thread2, sizeof(cpu_set_t), &cpus2 );

	int detected = 0;
	for( int iteration=1;; iteration++ ) {
			// We reset x=0, y=0 to repeat the experiment until we detect
			// reordering
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

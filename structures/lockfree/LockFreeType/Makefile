noatomic: noatomic.cpp
	g++ -lpthread -O3 -march=native noatomic.cpp -o noatomicDIFFNODE
	g++ -lpthread -O3 -DSAMENODE -march=native noatomic.cpp -o noatomicSAMENODE

# We have to select an march where __atomics__ exist as some compiler options do not
# have this lib and leads to a compiler error when using <atomics>
atomic: atomic.cpp
	g++ -lpthread -O3 atomic.cpp -march=native -o atomicRELAXED
	g++ -lpthread -O3 atomic.cpp -DMEMORY_BARRIER -march=native -o atomicACQREL

trivially_copyable: trivially_copyable.cpp
	g++ -lpthread -O3 trivially_copyable.cpp -o trivially_copyable

clean:
	rm -rf noatomicDIFFNODE noatomicSAMENODE 
	rm -rf atomicRELAXED atomicACQREL 
	rm -rf trivially_copyable

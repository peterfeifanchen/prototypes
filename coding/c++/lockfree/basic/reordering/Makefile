reordering: reordering.cpp
	g++ -lpthread -O2 $< -o $@ 

reorderingSAMECPU: reordering.cpp
	g++ -lpthread -O2 -DSAME_NODE $< -o $@ 

reorderingMFENCE: reordering.cpp
	g++ -lpthread -O2 -DMFENCE $< -o $@

reorderingACQREL: reordering.cpp
	g++ -lpthread -O2 -DACQREL $< -o $@

reorderingACQRELFENCE: reordering.cpp
	g++ -lpthread -O2 -DACQRELFENCE $< -o $@

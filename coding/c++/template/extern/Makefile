
OBJ = extern1.o extern2.o

%.o: %.c header.h
	g++ -c -o $@ $<

extern: $(OBJ)
	g++ -o $@ $^

clean:
	rm -rf *.o extern

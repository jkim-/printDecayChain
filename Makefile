LDFLAGS = $(shell root-config --libs)
INCFLAGS = $(shell root-config --incdir)

printDecayChain : printDecayChain.o
	g++ -std=c++11 $(LDFLAGS) $^ -o $@
printDecayChain.o : printDecayChain.cc
	g++ -std=c++11 -I$(INCFLAGS) -c $^ -o $@

clean: 
	@rm -rf printDecayChain printDecayChain.o

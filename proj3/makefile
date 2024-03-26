# use g++ compiler
cc = g++

# path for include directive and linter
path = ..

# compile with C++ 20 standard
flags = -std=c++2a
# compile with all warnings
flags += -Wall
# compile with path on #include path
flags += -I $(path)
# compile with instrumentation for gdb
flags += -g

# compilation to intermediary .o files
compile = $(cc) $(flags) -c -o

# final linked build to binary executable
link = $(cc) $(flags) -o

all: unix

unix: server client



server: shm_sup_server.o
	$(CXX) $< -o $(@:.o=) $(LDFLAGS)

client: shm_sup_client.o
	$(CXX) $< -o $(@:.o=) $(LDFLAGS)



lint:
	cpplint --root=.. src/* inc/*

clean:
	@echo clean ...
	
	@rm -f *.o
	
	@rm -f shm_sup_client
	@rm -f shm_sup_server

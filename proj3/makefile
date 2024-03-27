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

all : server client

server : shmServer.o
	$(link) $@ $^

client : shmClient.o
	$(link) $@ $^

shmServer.o : shmServer.cc shmServer.h shmConsts.h
	$(compile)  $@ $<

shmClient.o : shmClient.cc shmClient.h shmConsts.h
	$(compile)  $@ $<

lint : shmClient.cc shmClient.h shmServer.cc shmServer.h shmConsts.h
	cpplint --root=$(path) $^

# UTILITY: removes intermediary files
clean:
	$(RM) *.o
	$(RM) server
	$(RM) client
	$(RM) *.socket

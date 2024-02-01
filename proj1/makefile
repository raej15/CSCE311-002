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


calculate : main.o calculator.o
	$(link) $@ $^

main.o : main.cc
	$(compile)  $@ $<

calculator.o : calculator.cc calculator.h
	$(compile)  $@ $<

lint : calculator.cc calculator.h main.cc
	cpplint --root=$(path) $^

# UTILITY: removes intermediary files
clean:
	$(RM) *.o

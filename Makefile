
CXX = g++ -std=c++17 -Wall

all: run_test gnomes_timing

run_test: gnomes_test
	./gnomes_test

headers: rubrictest.hpp gnomes_types.hpp gnomes_algs.hpp

gnomes_test: headers gnomes_test.cpp
	${CXX} gnomes_test.cpp -o gnomes_test

gnomes_timing: headers gnomes_timing.cpp
	${CXX} gnomes_timing.cpp -o gnomes_timing

clean:
	rm -f gnomes_test gnomes_timing

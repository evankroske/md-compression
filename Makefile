CXXFLAGS += -g

all: md_data_generator mdshrink omeltchenko99_test discretize

test: test_omeltchenko99_simple

clean:
	rm -f *.o main omeltchenko99_test md_data_generator

test_omeltchenko99: omeltchenko99_test
	./omeltchenko99_test

test_md_data_generator: md_data_generator
	./md_data_generator

test_mdshrink: mdshrink md_data_generator
	./md_data_generator | sort > data.txt; \
	./mdshrink -c data.txt compressed; \
	./mdshrink -x compressed uncompressed.txt; \
	sort < uncompressed.txt | diff -q data.txt -

test_discretize: discretize
	./$< -e < data100.dat | ./$< -d -s 0.0001 > /dev/null

test_integration: discretize mdshrink
	./discretize -e data100.dat coordinates.txt; \
	./discretize -d -s 0.0001 < coordinates.txt | ./mdshrink -c > compressed; \
	./mdshrink -x < compressed | ./discretize -u -s 0.0001 > out.txt; \
	sort coordinates.txt > coordinates-sorted.txt; \
	sort out.txt > out-sorted.txt

test_omeltchenko99_simple: omeltchenko99-simple-test
	./omeltchenko99-simple-test

omeltchenko99_test: omeltchenko99.h omeltchenko99.o bin.h

mdshrink: omeltchenko99.o omeltchenko99.h

debug_mdshrink: omeltchenko99.o omeltchenko99.h
	g++ -g mdshrink.cpp omeltchenko99.o -o $@

md_data_generator: omeltchenko99.o

omeltchenko99-simple.o: omeltchenko99.o omeltchenko99.h

omeltchenko99-simple-test: omeltchenko99-simple.o omeltchenko99-simple.h omeltchenko99.o

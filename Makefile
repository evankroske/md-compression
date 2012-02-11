all: main md_data_generator mdshrink omeltchenko99_test

test: test_omeltchenko99

clean:
	rm -f *.o main omeltchenko99_test md_data_generator

test_omeltchenko99: omeltchenko99_test
	./omeltchenko99_test

test_md_data_generator: md_data_generator
	./md_data_generator

test_mdshrink: mdshrink md_data_generator
	./md_data_generator > data.txt; \
	./mdshrink -c data.txt compressed; \
	./mdshrink -x compressed uncompressed.txt; \
	diff data.txt uncompressed.txt

main: omeltchenko99.h omeltchenko99.o

omeltchenko99_test: omeltchenko99.h omeltchenko99.o bin.h

mdshrink: omeltchenko99.o omeltchenko99.h

md_data_generator: omeltchenko99.o

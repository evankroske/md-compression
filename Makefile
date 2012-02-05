all: main var_encoding_test md_data_generator

test: test_md_data_generator

clean:
	rm -f *.o main var_encoding_test

test_var_encoding: var_encoding_test
	./var_encoding_test

test_md_data_generator: md_data_generator
	./md_data_generator

main: omeltchenko99.h omeltchenko99.o

var_encoding_test: omeltchenko99.h omeltchenko99.o

all: main var_encoding_test

test: test_var_encoding

clean:
	rm -f *.o main var_encoding_test

test_var_encoding: var_encoding_test
	./var_encoding_test

main: omeltchenko99.h omeltchenko99.o

var_encoding_test: omeltchenko99.h omeltchenko99.o

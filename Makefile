all: vec_test
	echo "All tests passed"

vec_test:
	cc tests/vec.c -I.. -o vec_test && ./vec_test

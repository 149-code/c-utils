all: vec_test
	echo "All tests passed"

clean:
	rm build/*

vec_test:
	cc tests/vec.c -I.. -o build/vec_test && ./build/vec_test

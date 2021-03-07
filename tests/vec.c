#include "c-utils/vec.h"
#include <assert.h>
#include <string.h>

typedef struct Point
{
	long x;
	long y;
	long z;
} Point;

void struct_test()
{
	Point* arr = CU_vec_init(Point, 0);

	for (int i = 0; i < 10; i++)
	{
		CU_vec_push(arr, ((Point) {.x = i, .y = i, .z = i}));
	}

	assert(arr[3].x == 3);

	CU_vec_free(arr);
}

void realloc_test()
{
	int* arr = CU_vec_init(int, 0);

	for (int i = 0; i < 1000000; i++)
		CU_vec_push(arr, i);

	assert(arr[999999] == 999999);

	CU_vec_free(arr);
}

void push_arr_test()
{
	char* arr = CU_vec_init(char, 0);
	CU_vec_push_arr(arr, "Hello", 6);

	assert(strcmp(arr, "Hello") == 0);

	CU_vec_free(arr);
}

int main()
{
	struct_test();
	realloc_test();
	push_arr_test();
}

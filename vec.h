#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	size_t size;
	size_t len;
	size_t cap;
} __CU_vec_info;

#define __CU_get_vec_info_ptr(vec) ((__CU_vec_info*) (((void*) vec) - sizeof(__CU_vec_info)))

#define CU_vec_init(T, n) ((T*) __CU_vec_init(sizeof(T), n))
#define CU_vec_free(vec) (__CU_vec_free(__CU_get_vec_info_ptr(vec)))

#define CU_vec_len(vec) ((__CU_get_vec_info_ptr(vec))->len)
#define CU_vec_cap(vec) ((__CU_get_vec_info_ptr(vec))->cap)

#define CU_vec_push_arr(vec, arr, n)                                                               \
	do                                                                                         \
	{                                                                                          \
		for (int i = 0; i < n; i++)                                                        \
			CU_vec_push(vec, arr[i]);                                                  \
	}                                                                                          \
	while (false)

#define CU_vec_push(vec, n)                                                                        \
	do                                                                                         \
	{                                                                                          \
		__CU_vec_info* info_ptr = __CU_get_vec_info_ptr(vec);                              \
		info_ptr->len++;                                                                   \
		info_ptr = __CU_vec_should_realloc(info_ptr);                                      \
		vec = (void*) info_ptr + sizeof(__CU_vec_info);                                    \
		vec[info_ptr->len - 1] = n;                                                        \
	}                                                                                          \
	while (false)

#define CU_vec_pop(vec)                                                                            \
	({                                                                                         \
		__CU_vec_info* info_ptr = __CU_get_vec_info_ptr(vec);                              \
		info_ptr->len--;                                                                   \
		vec[info_ptr->len];                                                                \
	})

void* __CU_vec_init(size_t size, int num)
{
	void* alloc_ptr = malloc(sizeof(__CU_vec_info) + size * num);
	*(__CU_vec_info*) alloc_ptr = (__CU_vec_info){size, num, num};
	return alloc_ptr + sizeof(__CU_vec_info);
}

void __CU_vec_free(__CU_vec_info* vec)
{
	free(vec);
}

__CU_vec_info* __CU_vec_should_realloc(__CU_vec_info* vec)
{
	if (vec->len >= vec->cap)
	{

		vec->cap = (vec->cap * 3 / 2) + 1;
		vec = realloc(vec, vec->cap * vec->size + sizeof(__CU_vec_info));
	}

	return vec;
}

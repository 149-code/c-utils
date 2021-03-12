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
#define CU_subvec(vec, start, end) __CU_subvec(vec, start, end)

#define CU_vec_reserve(vec, new_size)                                                              \
	do                                                                                         \
	{                                                                                          \
		if (CU_vec_cap(vec) < new_size)                                                    \
		{                                                                                  \
			__CU_vec_info* info_ptr = __CU_get_vec_info_ptr(vec);                      \
			info_ptr = realloc(                                                        \
				info_ptr, sizeof(__CU_vec_info) + new_size * info_ptr->size);      \
			info_ptr->cap = new_size;                                                  \
			vec = ((void*) info_ptr) + sizeof(__CU_vec_info);                          \
		}                                                                                  \
	}                                                                                          \
	while (false)

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

#define CU_vec_foreach(i, vec, body)                                                               \
	do                                                                                         \
	{                                                                                          \
		for (int __CU_foreach_index = 0; __CU_foreach_index < CU_vec_len(vec);             \
			__CU_foreach_index++)                                                      \
		{                                                                                  \
			typeof(*vec) i = vec[__CU_foreach_index];                                  \
			body;                                                                      \
		}                                                                                  \
	}                                                                                          \
	while (false)

void* __CU_vec_init(size_t size, int num)
{
	void* alloc_ptr = malloc(sizeof(__CU_vec_info) + size * num);
	*(__CU_vec_info*) alloc_ptr = (__CU_vec_info){size, num, num};
	return alloc_ptr + sizeof(__CU_vec_info);
}

void* __CU_subvec(void* vec, size_t start, size_t end)
{
	__CU_vec_info* vec_info = __CU_get_vec_info_ptr(vec);
	int num_elems = end - start;
	void* ret = __CU_vec_init(vec_info->size, num_elems);
	memcpy(ret, vec + vec_info->size * start, vec_info->size * (num_elems));
	return ret;
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

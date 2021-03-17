#pragma once

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
} _CU_vec_info;

#define _CU_get_vec_info_ptr(vec) ((_CU_vec_info*) (((void*) vec) - sizeof(_CU_vec_info)))

#define CU_vec_init(T, n) ((T*) _CU_vec_init(sizeof(T), n))
#define CU_vec_free(vec) (_CU_vec_free(_CU_get_vec_info_ptr(vec)))

#define CU_vec_len(vec) ((_CU_get_vec_info_ptr(vec))->len)
#define CU_vec_cap(vec) ((_CU_get_vec_info_ptr(vec))->cap)
#define CU_subvec(vec, start, end) _CU_subvec(vec, start, end)

#define CU_vec_reserve(vec, new_size)                                                              \
	do                                                                                         \
	{                                                                                          \
		if (CU_vec_cap(vec) < new_size)                                                    \
		{                                                                                  \
			_CU_vec_info* info_ptr = _CU_get_vec_info_ptr(vec);                        \
			info_ptr = realloc(                                                        \
				info_ptr, sizeof(_CU_vec_info) + new_size * info_ptr->size);       \
			info_ptr->cap = new_size;                                                  \
			vec = ((void*) info_ptr) + sizeof(_CU_vec_info);                           \
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
		_CU_vec_info* info_ptr = _CU_get_vec_info_ptr(vec);                                \
		info_ptr->len++;                                                                   \
		info_ptr = _CU_vec_should_realloc(info_ptr);                                       \
		vec = (void*) info_ptr + sizeof(_CU_vec_info);                                     \
		vec[info_ptr->len - 1] = n;                                                        \
	}                                                                                          \
	while (false)

#define CU_vec_pop(vec)                                                                            \
	({                                                                                         \
		_CU_vec_info* info_ptr = _CU_get_vec_info_ptr(vec);                                \
		info_ptr->len--;                                                                   \
		vec[info_ptr->len];                                                                \
	})

#define CU_vec_popn(vec, n)                                                                        \
	{                                                                                          \
		_CU_vec_info* info_ptr = _CU_get_vec_info_ptr(vec);                                \
		info_ptr->len -= n;                                                                \
	}

#define CU_vec_remove_elem(vec, index) _CU_vec_remove_elems(_CU_get_vec_info_ptr(vec), index, 1)
#define CU_vec_remove_elems(vec, index, num_elems)                                                 \
	_CU_vec_remove_elems(_CU_get_vec_info_ptr(vec), index, num_elems)

#define CU_vec_foreach(i, vec, body)                                                               \
	do                                                                                         \
	{                                                                                          \
		for (int _CU_foreach_index = 0; _CU_foreach_index < CU_vec_len(vec);               \
			_CU_foreach_index++)                                                       \
		{                                                                                  \
			typeof(*vec) i = vec[_CU_foreach_index];                                   \
			body;                                                                      \
		}                                                                                  \
	}                                                                                          \
	while (false)

void* _CU_vec_init(size_t size, int num)
{
	void* alloc_ptr = malloc(sizeof(_CU_vec_info) + size * num);
	*(_CU_vec_info*) alloc_ptr = (_CU_vec_info){size, num, num};
	return alloc_ptr + sizeof(_CU_vec_info);
}

void* _CU_subvec(void* vec, size_t start, size_t num_elems)
{
	_CU_vec_info* vec_info = _CU_get_vec_info_ptr(vec);
	void* ret = _CU_vec_init(vec_info->size, num_elems);
	memcpy(ret, vec + vec_info->size * start, vec_info->size * (num_elems));
	return ret;
}

void _CU_vec_free(_CU_vec_info* vec)
{
	free(vec);
}

void _CU_vec_remove_elems(_CU_vec_info* info_ptr, size_t index, size_t num)
{
	void* vec_start = ((void*) info_ptr) + sizeof(_CU_vec_info);
	void* start_of_elems_to_remove = vec_start + info_ptr->size * index;
	void* end_of_elems_to_remove = start_of_elems_to_remove + info_ptr->size * num;

	memmove(start_of_elems_to_remove, end_of_elems_to_remove,
		(info_ptr->len - index - num) * info_ptr->size);
}

_CU_vec_info* _CU_vec_should_realloc(_CU_vec_info* vec)
{
	if (vec->len >= vec->cap)
	{

		vec->cap = (vec->cap * 3 / 2) + 1;
		vec = realloc(vec, vec->cap * vec->size + sizeof(_CU_vec_info));
	}

	return vec;
}

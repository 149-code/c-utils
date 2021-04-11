#pragma once

#include <string.h>
#include <stdio.h>

#include <c-utils/vec.h>

char* CU_io_read_file(FILE* fp)
{
	char* dynamic_buffer = CU_vec_init(char, 0);

	char c;
	while ((c = fgetc(fp)) != EOF)
		CU_vec_push(dynamic_buffer, c);

	int buffer_len = CU_vec_len(dynamic_buffer);
	char* ret = malloc(sizeof(char) * buffer_len + 1);
	memcpy(ret, dynamic_buffer, buffer_len);
	ret[buffer_len] = '\0';

	CU_vec_free(dynamic_buffer);
	return ret;
}

char* CU_io_read_till(FILE* fp, char delim)
{
	char* dynamic_buffer = CU_vec_init(char, 0);

	char c;
	while ((c = fgetc(fp)) != delim)
	{
		CU_vec_push(dynamic_buffer, c);

		if (c == EOF)
			break;
	}

	if (dynamic_buffer[0] == EOF)
		return NULL;

	int buffer_len = CU_vec_len(dynamic_buffer);
	char* ret = malloc(sizeof(char) * buffer_len + 1);
	memcpy(ret, dynamic_buffer, buffer_len);
	ret[buffer_len] = '\0';

	CU_vec_free(dynamic_buffer);
	return ret;
}

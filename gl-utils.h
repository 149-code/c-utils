#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

GLuint CU_gl_compile_shaders(int shader_type, const char* const* shader_src)
{
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, shader_src, NULL);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		GLint max_len = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_len);

		char buffer[max_len];
		glGetShaderInfoLog(shader, max_len, NULL, buffer);
		glDeleteShader(shader);

		fprintf(stderr, "SHADER COMPILATION FAILED: %s\n", buffer);
	}

	return shader;
}

GLuint CU_gl_basic_shader_prog(GLuint vert, GLuint frag)
{
	GLuint program = glCreateProgram();
	glAttachShader(program, vert);
	glAttachShader(program, frag);
	glLinkProgram(program);

	GLint success;
	glGetShaderiv(program, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint max_len = 0;
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &max_len);

		char buffer[max_len];
		glGetShaderInfoLog(program, max_len, NULL, buffer);
		glDeleteShader(program);

		fprintf(stderr, "SHADER LINKING FAILED: %s\n", buffer);
	}

	return program;
}

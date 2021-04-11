#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <c-utils/io.h>

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

GLFWwindow* CU_gl_setup_and_create_window(int width, int height, char* title)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize glfw\n");
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	glfwMakeContextCurrent(window);

	if (glewInit())
	{
		fprintf(stderr, "Failed to initialize glew\n");
		exit(1);
	}

	return window;
}

GLuint CU_gl_basic_prog_from_filename(char* vert_filename, char* frag_filename)
{
	FILE* vert_file = fopen(vert_filename, "r");
	if (vert_file == NULL)
	{
		fprintf(stderr, "Failed to open %s\n", vert_filename);
		exit(1);
	}

	FILE* frag_file = fopen(frag_filename, "r");
	if (frag_file == NULL)
	{
		fprintf(stderr, "Failed to open %s\n", vert_filename);
		exit(1);
	}

	const char* vert_source = CU_io_read_file(vert_file);
	const char* frag_source = CU_io_read_file(frag_file);

	GLuint vertex_shader = CU_gl_compile_shaders(GL_VERTEX_SHADER, &vert_source);
	GLuint fragment_shader = CU_gl_compile_shaders(GL_FRAGMENT_SHADER, &frag_source);
	GLuint program = CU_gl_basic_shader_prog(vertex_shader, fragment_shader);

	fclose(vert_file);
	fclose(frag_file);

	free((void*) vert_source);
	free((void*) frag_source);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}

#include "skippy/shader.h"

#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>


Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath) {
  const char *vertexShaderSource = read_file(vertexShaderPath);
  const char *fragmentShaderSource = read_file(fragmentShaderPath);

  unsigned int vertexShader = compile_shader(vertexShaderSource, GL_VERTEX_SHADER);
  unsigned int fragmentShader = compile_shader(fragmentShaderSource, GL_FRAGMENT_SHADER);

  mID = create_program(vertexShader, fragmentShader);
}

Shader::~Shader() {
  glDeleteProgram(mID);
}

void Shader::use() {
  glUseProgram(mID);
}

void Shader::set_float(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(mID, name.c_str()), value);
}

const char *Shader::read_file(const char *path) {
  FILE *file = fopen(path, "r");
  if (!file) {
    std::cerr << "Failed to open file: " << path << std::endl;
    return nullptr;
  }

  fseek(file, 0, SEEK_END);
  long length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *buffer = new char[length + 1];
  fread(buffer, 1, length, file);
  buffer[length] = '\0';

  fclose(file);

  return buffer;
}

unsigned int Shader::compile_shader(const char *source, unsigned int type) {
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  return shader;
}

unsigned int Shader::create_program(unsigned int vertexShader, unsigned int fragmentShader) {
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  int success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}


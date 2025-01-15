#include "skippy/shader.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath) {
  const auto vertexShaderSource = read_file(vertexShaderPath);
  const auto fragmentShaderSource = read_file(fragmentShaderPath);

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

std::string Shader::read_file(const char *path) {
  std::string file_content;
  std::ifstream file_stream;
  file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    file_stream.open(path);
    std::stringstream file_stream_buffer;
    file_stream_buffer << file_stream.rdbuf();
    file_stream.close();
    file_content = file_stream_buffer.str();
  } catch (std::ifstream::failure &e) {
    std::cerr << "Failed to read file: " << path << std::endl;
  }
  return file_content;
}

unsigned int Shader::compile_shader(const std::string source, unsigned int type) {
  const char *c_source = source.c_str();
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &c_source, NULL);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  std::cout << "Shader compiled successfully" << std::endl;

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


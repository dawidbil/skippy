#pragma once

#include <string>

class Shader {
public:
  Shader(const char *vertexShaderSource, const char *fragmentShaderSource);
  Shader(const Shader &other) = delete;
  Shader &operator=(const Shader &other) = delete;
  ~Shader();
  void use();
  void set_float(const std::string &name, float value) const;

private:
  std::string read_file(const char *path);
  unsigned int compile_shader(const std::string source, unsigned int type);
  unsigned int create_program(unsigned int vertexShader, unsigned int fragmentShader);

  unsigned int mID;
};


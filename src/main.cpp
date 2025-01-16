#include <cmath>
#include <filesystem>
#include <iostream>

#include <glad/glad.h>
// glad must be included before glfw
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "skippy/shader.h"
#include "skippy/stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int main() {
  float vertices[] = {
      // positions        // colors            // texture coords
      0.5f,  0.5f,  0.0f, 1.0f,  0.36f, 0.32f, 1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.32f, 0.32f, 1.0f,  1.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.17f, 0.54f, 0.17f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 0.9f,  0.9f,  0.9f,  0.0f, 1.0f  // top left
  };

  unsigned int indices[] = {
      0, 1, 3, 1, 2, 3,
  };

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  const auto resources_path =
      std::filesystem::path(__FILE__).parent_path().parent_path().append("res");
  const auto textures_path = resources_path / "textures";
  const auto vertex_shader_path = resources_path / "shader.vs";
  const auto fragment_shader_path = resources_path / "shader.fs";

  Shader shader(vertex_shader_path.string().c_str(),
                fragment_shader_path.string().c_str());

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  unsigned int vertex_array_object;
  glGenVertexArrays(1, &vertex_array_object);
  glBindVertexArray(vertex_array_object);

  unsigned int vertex_buffer_objects;
  glGenBuffers(1, &vertex_buffer_objects);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_objects);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coordinates attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  unsigned int element_buffer_objects;
  glGenBuffers(1, &element_buffer_objects);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_objects);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
  // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

  int width, height, nrChannels;
  unsigned char *data =
      stbi_load((textures_path / "container.jpg").string().c_str(), &width,
                &height, &nrChannels, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }

  stbi_image_free(data);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    float timeValue = glfwGetTime();
    float offset = 0.0f;
    // float offset = sin(timeValue) * 0.5f;
    shader.set_float("offset", offset);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(vertex_array_object);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

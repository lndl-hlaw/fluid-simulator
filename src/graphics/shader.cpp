#include "shader.hpp"
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

std::string getShaderCode(const char *filePath) {
  std::string shaderCode;
  std::ifstream shaderFile;
  // ensure ifstream objects can throw exceptions:
  shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try {
    // open files
    shaderFile.open(filePath);
    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << shaderFile.rdbuf();
    // close file handlers
    shaderFile.close();
    // convert stream into string
    shaderCode = vShaderStream.str();
  } catch (std::ifstream::failure const &e) {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ, path: " << filePath
              << std::endl;
    exit(1);
  }
  return shaderCode;
}

GLuint compileShader(GLuint type, const char *shaderCode) {
  int success;
  char infoLog[512];

  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &shaderCode, NULL);
  glCompileShader(shader);
  // print compile errors if any
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    exit(1);
  };
  return shader;
}

void graphics::shader::setFloat(const char *name, float value) const {
  int location = glGetUniformLocation(ID, name);
  glUniform1f(location, value);
}

void graphics::shader::setInt(const char *name, int value) const {
  int location = glGetUniformLocation(ID, name);
  glUniform1i(location, value);
}

void graphics::shader::setVector(const char *name, glm::vec3 vector) const {
  int location = glGetUniformLocation(ID, name);
  glUniform3f(location, vector.x, vector.y, vector.z);
}

void graphics::shader::setMatrix(const char *name, glm::mat4 matrix) const {
  int location = glGetUniformLocation(ID, name);
  glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

graphics::shader::shader(
    const char *vertexPath,
    const char *fragmentPath) { // 1. retrieve the vertex/fragment source code
                                // from filePath
  std::string vertexCode = getShaderCode(vertexPath);
  std::string fragmentCode = getShaderCode(fragmentPath);

  // 2. compile shaders
  GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
  GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

  // 3. shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  // print linking errors if any
  int success;
  char infoLog[512];

  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  // 4. delete the shaders as they're linked into our program now and no longer
  // necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void graphics::shader::use() const { glUseProgram(ID); }

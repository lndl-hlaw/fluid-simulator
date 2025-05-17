#include "texture.hpp"
#include <glad/glad.h>
#include "../utils/cuda_handle_error.cuh"
#include "cudaGL.h"
#include "cuda_gl_interop.h"
#include "cuda_runtime.h"

int graphics::texture::_totalTexCount = 0;

graphics::texture::texture(std::string name, int width, int height)
    : name(name), width(width), height(height) {
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);
  textureSamplerId = _totalTexCount++;
}

void graphics::texture::draw(const graphics::shader *shader) {
  shader->setInt(name.c_str(), textureSamplerId);
  glActiveTexture(GL_TEXTURE0 + textureSamplerId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  mesh.draw(shader);
}

graphics::cuTexture::cuTexture(std::string name, int width, int height,
                               unsigned int flags)
    : texture(name, width, height) {
  HANDLE_ERROR(cudaGraphicsGLRegisterImage(&cudaTexture, textureId, GL_TEXTURE_2D,
                                        flags));
}

cudaArray *graphics::cuTexture::mapTexData() {
  HANDLE_ERROR(cudaGraphicsMapResources(1, &cudaTexture));
  HANDLE_ERROR(
      cudaGraphicsSubResourceGetMappedArray(&mappedArray, cudaTexture, 0, 0));
  return mappedArray;
}

void graphics::cuTexture::unmapTexData() {
  HANDLE_ERROR(cudaDeviceSynchronize());
  HANDLE_ERROR(cudaGraphicsUnmapResources(1, &cudaTexture));
  HANDLE_ERROR(cudaPeekAtLastError());
}

#pragma once
#include "mesh/mesh_impl.hpp"
#include "shader.hpp"
#include <string>

namespace graphics {
class texture {
  static int _totalTexCount;

protected:
  graphics::texture_mesh mesh;
  unsigned int textureId;
  int textureSamplerId;
  std::string name;

  int width, height;

public:
  texture(std::string name, int width, int height);

  unsigned int getID() { return textureId; }
  std::string getName() { return name; }

  void draw(const graphics::shader *shader);
};

class cuTexture : public texture {
  cudaGraphicsResource *cudaTexture = nullptr;
  cudaArray *mappedArray = nullptr;

public:
  cuTexture(std::string name, int width, int height, unsigned int flags);
  cudaArray *mapTexData();
  void unmapTexData();
};
} // namespace graphics

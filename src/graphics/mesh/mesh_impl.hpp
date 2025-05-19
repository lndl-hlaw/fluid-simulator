#pragma once
#include "mesh.hpp"

namespace graphics {
// Mesh for 2D grid plane rendered with dedicated shared.
class XYplane_mesh : public elements_mesh
{
public:
	XYplane_mesh();
    void draw(const graphics::xyShader& shader, glm::mat4 view, glm::mat4 projection) const;
};

class sphereSpriteMesh : public points_mesh 
{
public:
    sphereSpriteMesh(std::vector<Vec3> &&vertices);
    void draw(const graphics::sphereSpriteShader& shader, glm::mat4 view, glm::mat4 projection, float radius, glm::vec3 minAABB, glm::vec3 maxAABB) const;
};

class texture_mesh : public elements_mesh {
  std::vector<glm::vec2> texCoords;
  unsigned int texBuffer;

public:
  texture_mesh();
  virtual void setupMesh() override;
};
}

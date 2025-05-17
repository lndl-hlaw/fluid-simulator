#pragma once
#include "../shader.hpp"
#include "driver_types.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace graphics {

using Vec3 = glm::vec3;

struct vertex {
    Vec3 position;
    Vec3 normal;
};

class mesh {
protected:
    unsigned int VBO, VAO;
public:
    mesh() {};
    virtual void draw(const graphics::shader *shader) const = 0;
    virtual void setupMesh() = 0;

    // Access mesh VBO data
    virtual size_t getSizeVBO() const = 0;
    unsigned int getVBO() const { return VBO; }
};

// Generic mesh used as a base class for points-based meshes types.
// Implements basic mesh interface. 
class points_mesh : public mesh {
protected:
    std::vector<Vec3> vertices;
public:
    points_mesh(std::vector<Vec3> &&vertices);

    void draw(const graphics::shader *shader) const override;
    virtual void setupMesh() override;
    size_t getSizeVBO() const override { return sizeof(Vec3); };
    std::vector<Vec3> getVertices() { return vertices; }
};

// Generic mesh used as a base class for indices-based meshes types.
// Implements basic mesh interface. 
class elements_mesh : public mesh{
protected:
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int EBO;

public:
    elements_mesh(){};
    elements_mesh(std::vector<vertex> &&vertices, std::vector<unsigned int> &&indices);
    void draw(const graphics::shader *shader) const override;
    virtual void setupMesh() override;
    size_t getSizeVBO() const override { return sizeof(vertex); };
    std::vector<vertex> getVertices() { return vertices; }
};

} // namespace graphics

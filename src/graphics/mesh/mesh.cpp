#include <glad/glad.h>
#include <vector>
#include "mesh.hpp"
#include "cudaGL.h"
#include "cuda_gl_interop.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "../shader.hpp"


graphics::elements_mesh::elements_mesh(
    std::vector<vertex> &&vertices,
    std::vector<unsigned int> &&indices
) : vertices(vertices), indices(indices) {}

graphics::points_mesh::points_mesh(std::vector<Vec3> &&vertices): vertices(vertices) {}

void graphics::elements_mesh::draw(const graphics::shader *shader) const {
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
                 GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void graphics::elements_mesh::setupMesh() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * getSizeVBO(), &vertices[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int),
               &indices[0], GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, getSizeVBO(), (void *)0);

  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, getSizeVBO(),
                        (void *)offsetof(vertex, normal));

  glBindVertexArray(0);
}

void graphics::points_mesh::draw(const graphics::shader *shader) const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, vertices.size());
    // glBindVertexArray(0);
}

void graphics::points_mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * getSizeVBO(), &vertices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void *)0);
    glBindVertexArray(0);
}

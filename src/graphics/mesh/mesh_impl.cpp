#include "mesh_impl.hpp"
#include <glad/glad.h>
#include <vector>


graphics::texture_mesh::texture_mesh() : elements_mesh() {
    vertices = {{{1, 1, 0}, {0, 0, 0}},
                {{1, -1, 0}, {0, 0, 0}},
                {{-1, -1, 0}, {0, 0, 0}},
                {{-1, 1, 0}, {0, 0, 0}}};
    indices = {0, 1, 3, 1, 2, 3};
    texCoords = {{1, 1}, {1, 0}, {0, 0}, {0, 1}};
    this->setupMesh();
}


graphics::XYplane_mesh::XYplane_mesh()
{
	vertices = { { {1,1,0}, {0,0,0} }, {{1,-1,0}, {0,0,0} }, {{-1,-1,0}, {0,0,0} }, {{-1,1,0} , {0,0,0} } };
	indices = { 0,1,3,1,2,3 };
	setupMesh();
}

void graphics::XYplane_mesh::draw(const graphics::xyShader& shader, glm::mat4 view, glm::mat4 projection) const
{
    shader.use();
    shader.setMatrix("viewInv", glm::inverse(view));
    shader.setMatrix("projInv", glm::inverse(projection));
    shader.setMatrix("fragView", view);
    shader.setMatrix("fragProj", projection);
    this->elements_mesh::draw(&shader);
}


graphics::sphereSpriteMesh::sphereSpriteMesh(std::vector<Vec3> &&vertices): 
    points_mesh(std::move(vertices)) 
{
    points_mesh::setupMesh(); 
}

void graphics::sphereSpriteMesh::draw(const graphics::sphereSpriteShader& shader, glm::mat4 view, glm::mat4 projection, float radius, glm::vec3 minAABB, glm::vec3 maxAABB) const {
    shader.use();
    shader.setMatrix("view", view);
    shader.setMatrix("projection", projection);
    shader.setFloat("radius", radius);
    shader.setVector("aabbMin", minAABB);
    shader.setVector("aabbMax", maxAABB);
    this->points_mesh::draw(&shader);
    
}

void graphics::texture_mesh::setupMesh() {
    elements_mesh::setupMesh();

    glGenBuffers(1, &texBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), &texCoords[0],
                 GL_DYNAMIC_DRAW);

    glBindVertexArray(VAO);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void *)0);
    glBindVertexArray(0);
}

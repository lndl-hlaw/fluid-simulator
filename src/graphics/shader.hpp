#pragma once
#include <glm/glm.hpp>

namespace graphics {
class shader {
protected:
    shader(const char *vertexPath, const char *fragmentPath);
    unsigned int ID;

public:
    void use() const;
    void setFloat(const char *name, float value) const;
    void setInt(const char *name, int value) const;
    void setVector(const char *name, glm::vec3 vector) const;
    void setMatrix(const char *name, glm::mat4 matrix) const;
};

class texShader : public shader {
public:
    texShader() : shader("shaders/tex_shader.vert", "shaders/tex_shader.frag") {}
};

class xyShader : public shader {
public:
    xyShader() : shader("shaders/xyplane_shader.vert", "shaders/xyplane_shader.frag") {}
};

class sphereSpriteShader : public shader {
public:
    sphereSpriteShader() : shader("shaders/sphere_sprite_shader.vert", "shaders/sphere_sprite_shader.frag") {}
};
} // namespace graphics

#pragma once
#include "../graphics/mesh/mesh.hpp"
#include <vector>
#include <glm/vec3.hpp>
#include <cstdlib>

namespace utils
{

inline std::vector<graphics::Vec3> generateRandomPoints(int n, glm::vec3 min, glm::vec3 max) {
    std::vector<graphics::Vec3> points(n);
    for(auto& point  : points) {
        point = glm::vec3 {
            min.x + static_cast<float>(rand())/static_cast<float>(RAND_MAX/(max.x - min.x)),
            min.y + static_cast<float>(rand())/static_cast<float>(RAND_MAX/(max.y - min.y)),
            min.z + static_cast<float>(rand())/static_cast<float>(RAND_MAX/(max.z - min.z))
        };
    }
    return std::move(points);
}
}

#include "global/constants.hpp"
#include "graphics/shader.hpp"
#include "utils/point_randomizer.hpp"
#include "graphics/mesh/mesh_impl.hpp"
#include "gui/window/window_controller.hpp"
#include "simulation/simulator.cuh"
#include "time.h"
#include <glad/glad.h> // https://glad.dav1d.de/
#include <GLFW/glfw3.h>

int main()
{
    glfwInit();
    srand(static_cast <unsigned> (time(0)));
    window::window_controller &windowController =
      window::window_controller::getInstance();
    
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glm::vec3 aabbMin = {XMIN, YMIN, ZMIN};
    glm::vec3 aabbMax = {XMAX, YMAX, ZMAX};
    graphics::XYplane_mesh xyPlane;
    graphics::xyShader xyShader;
    auto particles = utils::generateRandomPoints(PARTICLES_COUNT, aabbMin, aabbMax);
    graphics::sphereSpriteMesh sprites(std::move(particles));
    graphics::sphereSpriteShader spriteShader;

    simulation::Simulator simulator(sprites); 
    graphics::camera& camera = windowController.getCamera();

    while (!windowController.shouldClose()) {
        windowController.pollEventsAndHandleInput(); 

        glClearColor(0.3, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // simulation
        simulator.simulationStep();

        // rendering
        simulator.visualizePositions();
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.getProjectionMatrix();

        xyPlane.draw(xyShader, view, projection);
        sprites.draw(spriteShader, view, projection, PARTICLE_RADIUS_VIS, aabbMin, aabbMax);

        windowController.swapBuffers();
    }
}

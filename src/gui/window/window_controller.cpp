#include "window_controller.hpp"
#include "../../global/constants.hpp"
#include <iostream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

namespace window
{
	void window_size_callback(GLFWwindow* window, int width, int height);
	
	graphics::camera window::window_controller::camera = graphics::camera({ 0,1,3 }, { 0,0,1 });

	void window_controller::handleInput()
	{
		inputController.adjustParametersUsingInput(camera);
	}

	window_controller& window::window_controller::getInstance()
	{
		static window_controller instance;
		return instance;
	}

	window_controller::window_controller()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
		window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Fluid Simulator", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			exit(-1);
		}
		glfwMakeContextCurrent(window);
		fpsLastTime = glfwGetTime();
		fpsFrameCount = 0;
		inputController.setWindow(window);

		glfwSetWindowUserPointer(window, &inputController);
		glfwSetKeyCallback(window, inputController.handleUserInput);
		glfwSetCursorPosCallback(window, input_controller::mouse_callback);
		glfwSetMouseButtonCallback(window, input_controller::mouse_button_callback);
	}

	void window_controller::draw()
	{
		swapBuffers();
		pollEventsAndHandleInput();
	}

	window_controller::~window_controller()
	{
		glfwTerminate();
	}

	void window_controller::fpsCounter()
	{
		double fpsCurrentTime = glfwGetTime();
		double delta = fpsCurrentTime - fpsLastTime;
		if (delta >= 1.0)
		{
			double fps = double(fpsFrameCount) / delta;
			std::stringstream ss;
			ss << "Fluid Simulator " << " [" << fps << " FPS]";

			glfwSetWindowTitle(window, ss.str().c_str());
			fpsLastTime = fpsCurrentTime;
			fpsFrameCount = 0;
		}
		else
		{
			fpsFrameCount++;
		}
	}


}

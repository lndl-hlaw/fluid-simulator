#pragma once
#include "input_controller.hpp"

namespace window
{
	class window_controller
	{
		window_controller();

		GLFWwindow* window;
		double fpsLastTime, fpsFrameCount;
		static graphics::camera camera;

		void handleInput();

	public:
		input_controller inputController;
	
		static window_controller& getInstance();
		~window_controller();

		void fpsCounter();
		bool shouldClose() { return glfwWindowShouldClose(window); }
		void swapBuffers() { glfwSwapBuffers(window); }
		void pollEventsAndHandleInput() { fpsCounter(); glfwPollEvents(); /* HANDLE INPUT */ handleInput(); }
		graphics::camera& getCamera() { return camera; }
		static graphics::camera& getMainCamera() { return window_controller::getInstance().getCamera(); }

		void initImGui();
		void draw();

		glm::vec2 getMousePos() { return inputController.lastMousePos; }
	};
}

#pragma once
#include "../../graphics/camera.hpp"
#include <GLFW/glfw3.h>


namespace window
{
	class input_controller
	{
	public:

		static glm::vec2 frameCameraMovement;
		static glm::vec2 frameObjectMovement;
		static glm::vec2 frameObjectRotation;
		static glm::vec2 lastMousePos;

		struct clickAction
		{
			glm::vec2 pos;
			bool leftClickHappened;
			bool rightClickHappened;
		};
		static clickAction lastClick;
		bool leftMouseButtonPressed = false;
		bool rightMouseButtonPressed = false;
		GLFWwindow* _window;
		/// <summary>
		/// Handles given input on window
		/// </summary>
		/// <param name="window">pointer to current window</param>
		/// <param name="key">user input</param>
		/// <param name="scancode">input code</param>
		/// <param name="action">input action</param>
		/// <param name="mods">input mods</param>
		static void adjustParametersUsingInput(graphics::camera& camera);
		static void handleUserInput(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

		//void handle_mouse_click(float screen_xpos, float screen_ypos);
		void setWindow(GLFWwindow* window) { _window = window; };

	private:
		struct PressedKeys
		{
			bool W = false;
			bool S = false;
			bool A = false;
			bool D = false;
			bool SPACE = false;
			bool SHIFT = false;
			
			bool O = false;
			bool P = false;
			bool R = false;

			bool UP = false;
			bool DOWN = false;
			bool LEFT = false;
			bool RIGHT = false;

			bool PLUS = false;
			bool MINUS = false;
			bool LEFT_CTRL = false;
			bool DEL = false;
		};
		static PressedKeys pressedKeys;
	};
}

#include "input_controller.hpp"

namespace window
{
	input_controller::PressedKeys input_controller::pressedKeys = input_controller::PressedKeys();
	glm::vec2 input_controller::frameCameraMovement = { 0,0 };
	glm::vec2 input_controller::frameObjectMovement = { 0,0 };
	glm::vec2 input_controller::frameObjectRotation = { 0,0 };
	glm::vec2 input_controller::lastMousePos = { 0,0 };
	input_controller::clickAction input_controller::lastClick = { {0,0}, false, false };

	// GLFW keyboard callback function
	void input_controller::handleUserInput(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		// Get the input_controller instance from a custom GLFW pointer
		input_controller* controller = static_cast<input_controller*>(glfwGetWindowUserPointer(window));


		if (action == GLFW_PRESS)
		{
			// Movement
			switch (key)
			{
			case GLFW_KEY_W:
				controller->pressedKeys.W = true;
				break;
			case GLFW_KEY_S:
				controller->pressedKeys.S = true;
				break;
			case GLFW_KEY_A:
				controller->pressedKeys.A = true;
				break;
			case GLFW_KEY_D:
				controller->pressedKeys.D = true;
				break;
			case GLFW_KEY_P:
				controller->pressedKeys.P = true;
				break;
			case GLFW_KEY_O:
				controller->pressedKeys.O = true;
				break;
			case GLFW_KEY_R:
				controller->pressedKeys.R = true;
				break;
			case GLFW_KEY_SPACE:
				controller->pressedKeys.SPACE = true;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				controller->pressedKeys.SHIFT = true;
				break;
				// Rotation
			case GLFW_KEY_UP:
				controller->pressedKeys.UP = true;
				break;
			case GLFW_KEY_DOWN:
				controller->pressedKeys.DOWN = true;
				break;
			case GLFW_KEY_LEFT:
				controller->pressedKeys.LEFT = true;;
				break;
			case GLFW_KEY_RIGHT:
				controller->pressedKeys.RIGHT = true;
				break;
			case GLFW_KEY_EQUAL:
				controller->pressedKeys.PLUS = true;
				break;
			case GLFW_KEY_MINUS:
				controller->pressedKeys.MINUS = true;
				break;
			case GLFW_KEY_LEFT_CONTROL:
				controller->pressedKeys.LEFT_CTRL = true;
				break;
			case GLFW_KEY_DELETE:
				controller->pressedKeys.DEL = true;
				break;
			}
		}
		else if (action == GLFW_RELEASE)
		{
			switch (key)
			{
				// Movement
			case GLFW_KEY_W:
				controller->pressedKeys.W = false;
				break;
			case GLFW_KEY_S:
				controller->pressedKeys.S = false;
				break;
			case GLFW_KEY_A:
				controller->pressedKeys.A = false;
				break;
			case GLFW_KEY_D:
				controller->pressedKeys.D = false;
				break;
			case GLFW_KEY_P:
				controller->pressedKeys.P = false;
				break;
			case GLFW_KEY_O:
				controller->pressedKeys.O = false;
				break;
			case GLFW_KEY_R:
				controller->pressedKeys.R = false;
				break;
			case GLFW_KEY_SPACE:
				controller->pressedKeys.SPACE = false;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				controller->pressedKeys.SHIFT = false;
				break;
				// Rotation
			case GLFW_KEY_UP:
				controller->pressedKeys.UP = false;
				break;
			case GLFW_KEY_DOWN:
				controller->pressedKeys.DOWN = false;
				break;
			case GLFW_KEY_LEFT:
				controller->pressedKeys.LEFT = false;
				break;
			case GLFW_KEY_RIGHT:
				controller->pressedKeys.RIGHT = false;
				break;
			case GLFW_KEY_EQUAL:
				controller->pressedKeys.PLUS = false;
				break;
			case GLFW_KEY_MINUS:
				controller->pressedKeys.MINUS = false;
				break;
			case GLFW_KEY_LEFT_CONTROL:
				controller->pressedKeys.LEFT_CTRL = false;
				break;
			}
		}
	}

	void input_controller::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		input_controller* controller = static_cast<input_controller*>(glfwGetWindowUserPointer(window));
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			double xpos, ypos;
			//glm::vec2 cursorPos = gui::main_gui::getInstance().getMousePosOnFrame();
			glfwGetCursorPos(window, &xpos, &ypos);
			//controller->lastClick = { {cursorPos.x, cursorPos.y }, true, true };
			controller->lastClick = { {xpos, ypos}, true, false };
			controller->leftMouseButtonPressed = true;
		}
		else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			controller->leftMouseButtonPressed = false;
		}

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			controller->lastClick = { {xpos, ypos}, false, true };
		}
	}

	void input_controller::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		static bool firstMouse = false;
		static float lastX = 0;
		static float lastY = 0;

		input_controller* controller = static_cast<input_controller*>(glfwGetWindowUserPointer(window));


		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		lastMousePos = { xpos, ypos };

		if (pressedKeys.SHIFT && !pressedKeys.SPACE)
			controller->frameCameraMovement += glm::vec2(xoffset, yoffset);
		// else if((viewMode == geometry::viewMode::objectMode ||
		// 	viewMode == geometry::viewMode::pointerMode) &&
		// 	controller->leftMouseButtonPressed /*&& !pressedKeys.LEFT_CTRL*/)
		// 	controller->frameObjectMovement += glm::vec2(xoffset, yoffset);
		// else if(viewMode == geometry::viewMode::rotationMode &&
		// 	controller->leftMouseButtonPressed)
		// 	controller->frameObjectRotation += glm::vec2(xoffset, yoffset);
	}

	// Check pressed keys every frame
	void input_controller::adjustParametersUsingInput(graphics::camera& camera)
	{
		if (pressedKeys.W)
			camera.moveForward();
		if (pressedKeys.S)
			camera.moveBack();
		if (pressedKeys.A)
			camera.moveLeft();
		if (pressedKeys.D)
			camera.moveRight();
		if (pressedKeys.SPACE && !pressedKeys.SHIFT)
			camera.ascend();
		if (pressedKeys.SHIFT && pressedKeys.SPACE)
			camera.descend();
		if (pressedKeys.UP)
			camera.rotateUp();
		if (pressedKeys.DOWN)
			camera.rotateDown();
		if (pressedKeys.LEFT)
			camera.rotateLeft();
		if (pressedKeys.RIGHT)
			camera.rotateRight();
		// if (pressedKeys.O && pressedKeys.LEFT_CTRL)
		// {
		// 	state.setViewMode(geometry::viewMode::objectMode);
		// 	state.isCursorVisible = false;
		// }
		// if (pressedKeys.P && pressedKeys.LEFT_CTRL)
		// {
		// 	state.setViewMode(geometry::viewMode::pointerMode);
		// 	state.isCursorVisible = true;
		// }
		// if (pressedKeys.R && pressedKeys.LEFT_CTRL)
		// {
		// 	state.setViewMode(geometry::viewMode::rotationMode);
		// 	state.isCursorVisible = false;
		// }
		// if (pressedKeys.DEL)
		// {
		// 	state.deleteSelected();
		// 	pressedKeys.DEL = false;
		// }
		// state.isMultipleSelection = pressedKeys.LEFT_CTRL;
		//
		//if(frameCameraMovement.x != 0 || frameCameraMovement.y != 0)
		camera.ProcessMouseMovement(frameCameraMovement.x, frameCameraMovement.y);
		// if(frameObjectMovement.x != 0 || frameObjectMovement.y != 0)
		// 	camera.moveSelectedObjects(frameObjectMovement.x, frameObjectMovement.y);
		// if (frameObjectRotation.x != 0 || frameObjectRotation.y != 0)
		// 	camera.rotateSelectedObjects(frameObjectRotation.x, frameObjectRotation.y);
		// if (lastClick.leftClickHappened) {
		// 	camera.checkObjectMousePicking(lastClick.pos.x, lastClick.pos.y);
		// 	lastClick.leftClickHappened = false;
		// }
		// else if (lastClick.rightClickHappened) {
		// 	camera.setPointerOnPosition(lastClick.pos.x, lastClick.pos.y);
		// 	lastClick.rightClickHappened = false;
		// }
		frameCameraMovement = { 0,0 };
		frameObjectMovement = { 0,0 };
		frameObjectRotation = { 0,0 };
	}
}
